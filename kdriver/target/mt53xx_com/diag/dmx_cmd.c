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
 * $RCSfile: dmx_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_cmd.c
 *  Demux CLI commands
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "gcpu_if.h"
#include "dmx_mm_if.h"
#include "fvr_if.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "vdec_drvif.h"
#include "aud_if.h"
#include "aud_drvif.h"
#include "fbm_drvif.h"
#include "stc_drvif.h"
#include "x_drv_cli.h"
#include "x_stl_lib.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_rand.h"
#include "x_os.h"
#include "x_bim.h"

#if defined(CC_TRUSTZONE_SUPPORT) && defined(CC_SVP_SUPPORT)
#include "tz_if.h"
#endif
//---------------------------------------------------------------------
// LOG macro related

// #define DEFINE_IS_LOG    CLI_IsLog
#define DEFINE_IS_LOG   DMX_IsLog
#include "x_debug.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

LINT_SAVE_AND_DISABLE
CLIMOD_DEBUG_FUNCTIONS(DMX)
LINT_RESTORE

LINT_SUPPRESS_SYMBOL(818, aszArgv)

//#define CC_DMX_DEBUG
//#define CC_DMX_EMULATION

//#define __MODEL_slt__

extern void _DMX_SetPtsInfo(BOOL fgEnableVideoPts,BOOL fgEnableAudioPts);
extern void _DmxSetPcrInfo(BOOL fgEnable);
extern INT32 STC_SetPtsDrift(UCHAR ucStcId, INT32 i4Drift);
extern INT32 STC_GetPtsDrift(UCHAR ucStcId);
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define DMX_DEFAULT_ES_FIFO_SIZE                0x100000
#define DMX_DEFAULT_PSI_FIFO_SIZE               0x10000
#ifndef CC_MT5391_AUD_3_DECODER
#define NUM_AUDIO_DEVICE                        2
#else
#define NUM_AUDIO_DEVICE                        3
#endif
#define REG_DMX_RESET                           0
#define REG_PWM                                 (BIM_BASE + 0x230)

#define CAP_THREAD_NAME                         "Capture"
#define CAP_THREAD_STACK_SIZE                   8192
#define CAP_THREAD_PRIORITY                     60

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/** Brief of PWM_REGISTER_T
 */
typedef struct
{
    UINT32 u4Enable     :    1;
    UINT32 u4LoadDmx    :    1;
    UINT32              :    6;
    UINT32 u4Prescaler  :    8;
    UINT32 u4High       :    8;
    UINT32 u4Resolution :    8;
} PWM_REGISTER_T;

/// Section info
typedef struct
{
    UINT16 u2SectionLength;
    UINT16 u2TableIdExtension;
    UINT8 u1TableId;
    UINT8 u1VersionNumber;
    UINT8 u1SectionNumber;
    UINT8 u1LastSectionNumber;
    BOOL fgSectionSyntaxIndicator;
    BOOL fgPrivateIndicator;
    BOOL fgCurrentNextIndicator;
} SECTION_INFO_T;

#ifndef NDEBUG
typedef struct
{
    UINT32 u4LogMask;
    UINT8* pu1LogName;
} DMX_LOGMASK_T;
#endif
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
EXTERN UINT8 SerPollGetChar(void);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/// TS index
static UINT8 _u1TsIndex = 0;
static UINT8 _u1PidIndex = 0;
static UINT8 _u1FilterIndex = 0;
static UINT32 _u4CaptureBufAddr = 0;
static UINT32 _u4CaptureBufSize = 0;
static BOOL _fgCaptureEnable = FALSE;
static FBM_POOL_T *_prDmxFbmPool = NULL;

#ifndef NDEBUG
static DMX_LOGMASK_T _arLogMask[] =
{
    {(1U << 16), "INIT"},        // DMX_LOG_INIT
    {(1U << 17), "PID"},         // DMX_LOG_PID
    {(1U << 18), "BUFFER"},      // DMX_LOG_BUFFER
    {(1U << 19), "PSI"},         // DMX_LOG_PSI
    {(1U << 20), "PES"},         // DMX_LOG_PES
    {(1U << 21), "CALLBACK"},    // DMX_LOG_CALLBACK
    {(1U << 22), "UP"},          // DMX_LOG_UP
    {(1U << 23), "PATTERN"},     // DMX_LOG_PATTERN
    {(1U << 24), "PCR"},         // DMX_LOG_PCR
    {(1U << 25), "RECORD"},      // DMX_LOG_RECORD
    {(1U << 26), "PVRPLAY"},     // DMX_LOG_PVRPLAY
    {(1U << 27), "SCRAMBLE"},    // DMX_LOG_SCRAMBLE
    {(1U << 28), "DDI"},         // DMX_LOG_DDI
    {(1U << 29), "MUL"},         // DMX_LOG_MUL
    {(1U << 30), "AV"},          // DMX_LOG_AV
    {(1U << 31), "FUNCTION"},    // DMX_LOG_FUNCTION
};
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _ShowPidInfo(UINT8 u1Pidx, const DMX_PID_T* prPid)
{
    UINT32 u4DataSize;
    CHAR szBuf[64];
    CHAR* pszType;
    CHAR* pszMode;

    ASSERT(prPid != NULL);

    // Index, valid, PCR, callback, PID
    Printf("\nIndex: %u, Enable: %s, PCR: %s, Callback: %s, PID: %u(0x%x)\n",
           u1Pidx, (prPid->fgEnable ? "Y" : "N"),
           ((prPid->ePcrMode != DMX_PCR_MODE_NONE) ? "Y" : "-"),
           ((prPid->pfnNotify != NULL) ? "Y" : "-"),
           prPid->u2Pid, prPid->u2Pid);

    // PID type
    switch (prPid->ePidType)
    {
    case DMX_PID_TYPE_NONE:
        pszType = "PCR";
        break;

    case DMX_PID_TYPE_PSI:
        pszType = "PSI";
        break;

    case DMX_PID_TYPE_PES:
        pszType = "PES";
        break;

    case DMX_PID_TYPE_ES_VIDEO:
        pszType = "VIDEO ES";
        break;

    case DMX_PID_TYPE_ES_AUDIO:
        pszType = "AUDIO ES";
        break;

    case DMX_PID_TYPE_TS_RAW:
        pszType = "RAW TS";
        break;

    default:
        pszType = "UNKNOWN";
        break;
    }

    // Descramble mode
    switch (prPid->eDescMode)
    {
    case DMX_DESC_MODE_NONE:
        pszMode = "NONE";
        break;

    case DMX_DESC_MODE_DES_ECB:
        pszMode = "DES ECB";
        break;

    case DMX_DESC_MODE_DES_CBC:
        pszMode = "DES CBC";
        break;

    case DMX_DESC_MODE_3DES_ECB:
        pszMode = "3DES ECB";
        break;

    case DMX_DESC_MODE_3DES_CBC:
        pszMode = "3DES CBC";
        break;

    case DMX_DESC_MODE_DVB:
        pszMode = "DVB";
        break;

    case DMX_DESC_MODE_DVB_CONF:
        pszMode = "DVB CONF";
        break;

    case DMX_DESC_MODE_MULTI2_BIG:
        pszMode = "MULTI2_BIG";
        break;

    case DMX_DESC_MODE_MULTI2_LITTLE:
        pszMode = "MULTI2_LITTLE";
        break;

    case DMX_DESC_MODE_AES_ECB:
        pszMode = "AES_ECB";
        break;

    case DMX_DESC_MODE_AES_CBC:
        pszMode = "AES_CBC";
        break;

    default:
        pszMode = "UNKNOWN";
        break;
    }

    // Steering
    szBuf[0] = '\0';
    if ((prPid->u1SteerMode & DMX_STEER_TO_FTUP) != 0)
    {
        x_strncat(szBuf, " uP", sizeof(" uP"));
    }
    if ((prPid->u1SteerMode & DMX_STEER_TO_PVR) != 0)
    {
        x_strncat(szBuf, "PVR", sizeof("PVR"));
    }

    Printf("  Descramble mode: %s, Key index: %u, Steering:%s, Type: %s\n",
           pszMode, prPid->u1KeyIndex, szBuf, pszType);

    // Buffer
    Printf("  Buffer: 0x%08x - 0x%08x (0x%08x)\n",
           prPid->u4BufAddr, prPid->u4BufAddr + prPid->u4BufSize,
           prPid->u4BufSize);

    // Data pointers
    u4DataSize = DATASIZE(prPid->u4Rp, prPid->u4Wp, prPid->u4BufSize);
    Printf("  RP: 0x%08x, WP: 0x%08x, Data size: %u\n",
           prPid->u4Rp, prPid->u4Wp, u4DataSize);
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _ShowFilterInfo(UINT8 u1Index, UINT8 u1Bank,
                            const DMX_FILTER_T* prFilter)
{
    DMX_PID_T rPid;
    CHAR* pszMode;

    ASSERT(prFilter != NULL);

    VERIFY(DMX_CLIGetPid(prFilter->u1Pidx, DMX_PID_FLAG_PID, &rPid));

    // Index, valid, pidx, pid, offset
    Printf("\nIndex: %u, Bank: %u, Enable: %s, PID index: %u, PID: %u(0x%x)\n",
           u1Index, u1Bank, (prFilter->fgEnable ? "Y" : "N"), prFilter->u1Pidx,
           rPid.u2Pid, rPid.u2Pid);

    // Translate mode to string
    switch (prFilter->eMode)
    {
    case DMX_FILTER_MODE_SHORT:
        pszMode = "SHORT";
        break;

    case DMX_FILTER_MODE_14_2:
        pszMode = "14+2";
        break;

    case DMX_FILTER_MODE_LONG:
        pszMode = "LONG";
        break;

    case DMX_FILTER_MODE_POS_NEG:
        pszMode = "POS+NEG";
        break;

    default:
        pszMode = "UNKNWON";
        break;
    }

    // Show offset, CRC checking, mode
    Printf("  Offset: %u, CRC: %s, Mode: %s\n", prFilter->u1Offset,
           (prFilter->fgCheckCrc ? "Y" : "N"), pszMode);

    // Show pattern and mask
    Printf("  Pattern: 0x%08x-0x%08x, Mask: 0x%08x-0x%08x\n",
           prFilter->au4Data[0], prFilter->au4Data[1], prFilter->au4Mask[0],
           prFilter->au4Mask[1]);
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _ShowGenericFilterInfo(UINT8 u1Index, const DMX_FILTER_GENERIC_T *prFilter)
{
    DMX_PID_T rPid;
    UINT8 i;
    /* FIXME: Put the next statement into dmx_if.h or dmx_mm_if.h. */
#define DMX_MAX_GENERIC_FILTER_LEN              16

    ASSERT(prFilter != NULL);

    VERIFY(DMX_CLIGetPid(prFilter->u1Pidx, DMX_PID_FLAG_PID, &rPid));

    // Filter Index, valid, CRC on/off, Offset, Pidx, PID value
    Printf("FltIdx: %u, Enable: %c, CRC: %c, Offset: %u, Pidx: %u, PID: 0x%x\n",
           u1Index, (prFilter->fgEnable ? 'Y' : 'N'),
           (prFilter->fgCheckCrc ? 'Y' : 'N'), prFilter->u1Offset,
           prFilter->u1Pidx, rPid.u2Pid);

    // Show pattern
    Printf("Pattern:  0x");
    for (i = 0; i < DMX_MAX_GENERIC_FILTER_LEN; i++)
    {
        Printf("%02X", prFilter->au1Data[i]);
        if ((i % 4) == 3)
        {
            if (i != (DMX_MAX_GENERIC_FILTER_LEN - 1))
            {
                Printf("-");
            }
            else
            {
                Printf("\n");
            }
        }
    }

    // Show mask
    Printf("Mask:     0x");
    for (i = 0; i < DMX_MAX_GENERIC_FILTER_LEN; i++)
    {
        Printf("%02X", prFilter->au1Mask[i]);
        if ((i % 4) == 3)
        {
            if (i != (DMX_MAX_GENERIC_FILTER_LEN - 1))
            {
                Printf("-");
            }
            else
            {
                Printf("\n");
            }
        }
    }

    // Show Positive/Negative mode
    Printf("P/N(0/1): 0x");
    for (i = 0; i < DMX_MAX_GENERIC_FILTER_LEN; i++)
    {
        Printf("%02X", prFilter->au1PosNeg[i]);
        if ((i % 4) == 3)
        {
            if (i != (DMX_MAX_GENERIC_FILTER_LEN - 1))
            {
                Printf("-");
            }
            else
            {
                Printf("\n\n");
            }
        }
    }
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _ShowPidCounters(UINT8 u1Pidx)
{
    DMX_PID_COUNTERS_T rCounters;

    // Get PID counters
    if (!DMX_GetPidCounters(u1Pidx, &rCounters))
    {
        Printf("Fail to get counters of PID index %u\n", u1Pidx);
        return;
    }

    // Show PID counters
    if (rCounters.u4PicCount != 0)
    {
        Printf("\nPID index: %u, PES count: %u, Pictures: %u, Sections: %u\n",
               u1Pidx, rCounters.u4PesCount, rCounters.u4PicCount,
               rCounters.u4SecCount);
    }
    else
    {
        Printf("\nPID index: %u, PES count: %u, Sections: %u\n", u1Pidx,
               rCounters.u4PesCount, rCounters.u4SecCount);
    }

    Printf("-------------------------------------------------------------\n");
    Printf("DBM: %u, DESC: %u, PCR: %u, Steer: %u\n", rCounters.u4DbmErrors,
           rCounters.u4DescErrors, rCounters.u4PcrErrors,
           rCounters.u4SteerErrors);
    Printf("FTuP: %u, Memory: %u, PVR: %u\n", rCounters.u4FTuPErrors,
           rCounters.u4MemoryErrors, rCounters.u4PvrErrors);
    Printf("-------------------------------------------------------------\n");
    Printf("Total errors: %u\n", rCounters.u4TotalErrors);
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------

#if !defined(CC_MT5890) && !defined(CC_MT5882)
extern UINT8 _u1DmxAudActivePidx;
#endif

static void _QueryPid(UINT8 u1Pidx)
{
    static CHAR szMsg[256], szBuf[128];
    DMX_PID_T rPid;
    DMX_PID_COUNTERS_T rCounters;
    CHAR* pszType;
    UINT32 u4DataSize;

    // Get PID info
    if (!DMX_CLIGetPid(u1Pidx, DMX_PID_FLAG_ALL, &rPid))
    {
        return;
    }

    if (!rPid.fgEnable)
    {
        return;
    }

    // PID type
    switch (rPid.ePidType)
    {
    case DMX_PID_TYPE_NONE:
        pszType = "PCR";
        break;

    case DMX_PID_TYPE_PSI:
        pszType = "PSI";
        break;

    case DMX_PID_TYPE_PES:
        pszType = "PES";
        break;

    case DMX_PID_TYPE_ES_VIDEO:
        pszType = "VIDEO";
        break;

    case DMX_PID_TYPE_ES_AUDIO:
        pszType = "AUDIO";
        break;

    case DMX_PID_TYPE_TS_RAW:
        pszType = "RAWTS";
        break;

    case DMX_PID_TYPE_AV_PES:
        pszType = "AVPES";
        break;

    default:
        pszType = "UNKNOWN";
        break;
    }

    x_snprintf(szMsg, 256, "Pidx: %u, TS_idx: %d, PID: 0x%x, %s", u1Pidx, rPid.u1TsIndex, rPid.u2Pid, pszType);

    // PID counters
    if (!DMX_GetPidCounters(u1Pidx, &rCounters))
    {
        return;
    }

    if (rCounters.u4PesCount > 0)
    {
        x_snprintf(szBuf, 128, ", PES: %u", rCounters.u4PesCount);
        x_strncat(szMsg, szBuf, 128);
    }
    if (rCounters.u4PicCount > 0)
    {
        x_snprintf(szBuf, 128, ", Pic: %u", rCounters.u4PicCount);
        x_strncat(szMsg, szBuf, 128);
    }
    if (rCounters.u4SecCount > 0)
    {
        x_snprintf(szBuf, 128, ", Sec: %u", rCounters.u4SecCount);
        x_strncat(szMsg, szBuf, 128);
    }

    u4DataSize = DATASIZE(rPid.u4Rp, rPid.u4Wp, rPid.u4BufSize);
    x_snprintf(szBuf, 128, ", Errors: %u, data: %u ", rCounters.u4TotalErrors, u4DataSize);
    x_strncat(szMsg, szBuf, 128);

#if !defined(CC_MT5890) && !defined(CC_MT5882)
    if (_u1DmxAudActivePidx == u1Pidx)
    {
        x_snprintf(szBuf, 128, "ActivePid!\n");
    }
    else
    {
        x_snprintf(szBuf, 128, "\n");
    }
    x_strncat(szMsg, szBuf, 128);
#endif

    // Show PID info
    Printf("%s\r\n", szMsg);
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _QueryMulPid(UINT8 u1Pidx)
{
    DMX_MUL_PIDINFO_T rMulPid;

    // Get MULPID info
    if (!DMX_MUL_GetMulPidInfo(u1Pidx, &rMulPid))
    {
        return;
    }

    if (!rMulPid.fgUsed)
    {
        return;
    }

    // Show PID info
    Printf("Pidx: %u, InstId: %d.\n", u1Pidx, rMulPid.u1InstId);

    return;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _QueryMulInst(UINT8 u1InstId)
{
    DMX_MUL_INSTINFO_T rMulInst;
    static CHAR szMsg[256], szBuf[128];
    CHAR* pszType;

    // Get MULPID info
    if (!DMX_MUL_GetMulInstInfo(u1InstId, &rMulInst))
    {
        return;
    }

    if (!rMulInst.fgUsed)
    {
        return;
    }

    // ConneType type
    switch (rMulInst.eConnType)
    {
    case DMX_CONN_TYPE_TUNER_0:
        pszType = "TUNER_0   ";
        break;

    case DMX_CONN_TYPE_TUNER_1:
        pszType = "TUNER_1   ";
        break;

    case DMX_CONN_TYPE_TUNER_2:
        pszType = "TUNER_2   ";
        break;

    case DMX_CONN_TYPE_BUFAGENT_0:
        pszType = "BUFAGENT_0";
        break;

    case DMX_CONN_TYPE_BUFAGENT_1:
        pszType = "BUFAGENT_1";
        break;

    case DMX_CONN_TYPE_BUFAGENT_2:
        pszType = "BUFAGENT_2";
        break;

    case DMX_CONN_TYPE_BUFAGENT_3:
        pszType = "BUFAGENT_3";
        break;

    case DMX_CONN_TYPE_BUFAGENT_4:
        pszType = "BUFAGENT_4";
        break;

    case DMX_CONN_TYPE_NONE:
        pszType = "NONE      ";
        break;
    default:
        pszType = "UNKNOWN   ";
        break;
    }

    x_snprintf(szMsg, 256, "InstId: %u, TS_idx: %d, ConnectType: %s", u1InstId, rMulInst.u1TsIdx, pszType);

    // ConneType type
    switch (rMulInst.eInputType)
    {

    case DMX_IN_NONE:
        pszType = "NONE        ";
        break;

    case DMX_IN_DIAG:
        pszType = "DIAG        ";
        break;

    case DMX_IN_BROADCAST_TS:
        pszType = "BROADCAST_TS";
        break;

    case DMX_IN_PLAYBACK_TS:
        pszType = "PLAYBACK_TS ";
        break;

    case DMX_IN_PLAYBACK_PS:
        pszType = "PLAYBACK_PS ";
        break;

    case DMX_IN_PLAYBACK_MM:
        pszType = "PLAYBACK_MM ";
        break;

    case DMX_IN_PVR_TS:
        pszType = "PVR_TS      ";
        break;

    case DMX_IN_PLAYBACK_ES:
        pszType = "PLAYBACK_ES ";
        break;

    default:
        pszType = "UNKNOWN     ";
        break;
    }

    x_snprintf(szBuf, 128, ", InputType: %s", pszType);
    x_strncat(szMsg, szBuf, 128);

    // VideoType
    switch (rMulInst.eVideoType)
    {
    case DMX_VIDEO_MPEG:
        pszType = "MPEG2     ";
        break;

    case DMX_VIDEO_H264:
        pszType = "H264      ";
        break;

    case DMX_VIDEO_AVS:
        pszType = "AVS       ";
        break;

    case DMX_VIDEO_VC1:
        pszType = "VC1       ";
        break;

    case DMX_VIDEO_MPEG4:
        pszType = "MPEG4     ";
        break;

    case DMX_VIDEO_WMV7:
        pszType = "WMV7      ";
        break;

    case DMX_VIDEO_WMV8:
        pszType = "WMV8      ";
        break;

    case DMX_VIDEO_WMV9:
        pszType = "WMV9      ";
        break;

    case DMX_VIDEO_MP4_IN_WMV:
        pszType = "MP4_IN_WMV";
        break;

    case DMX_VIDEO_RV:
        pszType = "RV        ";
        break;

    case DMX_VIDEO_VP6:
        pszType = "VP6       ";
        break;

    case DMX_VIDEO_VP8:
        pszType = "VP8       ";
        break;

    case DMX_VIDEO_NONE:
        pszType = "NONE      ";
        break;

    default:
        pszType = "UNKNOWN   ";
        break;
    }

    x_snprintf(szBuf, 128, ", VideoType: %s", pszType);
    x_strncat(szMsg, szBuf, 128);

    // ConneType type
    switch (rMulInst.eTSFmt)
    {
    case DMX_TSFMT_NONE:
        pszType = "NONE       ";
        break;

    case DMX_TSFMT_188:
        pszType = "188        ";
        break;

    case DMX_TSFMT_192:
        pszType = "192        ";
        break;

    case DMX_TSFMT_192_ENCRYPT:
        pszType = "192_ENCRYPT";
        break;

    case DMX_TSFMT_TIMESHIFT:
        pszType = "TIMESHIFT  ";
        break;

    case DMX_TSFMT_130:
        pszType = "130        ";
        break;

    default:
        pszType = "UNKNOWN    ";
        break;
    }

    x_snprintf(szBuf, 128, ", TSFmt: %s", pszType);
    x_strncat(szMsg, szBuf, 128);

    // Show PID info
    Printf("%s.\n", szMsg);

    return;
}

#if 0

//-----------------------------------------------------------------------------
/** _DumpBuffer
 *  Dump a buffer
 *
 *  @param  au1Buf          The buffer
 *  @param  u4Size          Size of the buffer
 *  @param  u4ByesPerLine   Bytes per print line
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DumpBuffer(const UINT8 au1Buf[], UINT32 u4Size,
                        UINT32 u4BytesPerLine)
{
    UINT32 i, j;

    ASSERT(au1Buf != NULL);

    j = 0;
    for (i = 0; i < u4Size; i++)
    {
        if (j == 0)
        {
            Printf("\n%04x:", i);
        }

        Printf(" %02x", au1Buf[i]);

        if (j == 7)
        {
            Printf(" |");
        }

        j++;
        if (j == u4BytesPerLine)
        {
            j = 0;
        }
    }
    Printf("\n");
}

#endif  // 0

//=====================================================================
// Command functions
//=====================================================================


// These functions exist only in SLT mode
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_Diag(INT32 i4Argc, const CHAR ** aszArgv)
{
    INT32 i4Ret;

    Printf("Self diagnostic testing...\n");
    i4Ret = DMX_Diag();
    Printf("%s\n", (i4Ret == 0) ? "OK!" : "Failed");

    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_JChipDiag(INT32 i4Argc, const CHAR **aszArgv)
{
    INT32 i4Ret;

    Printf("J-Chip functionality test...\n");
    if ((BIM_READ32(0x664) & 0x4) == 0x4) // demod does not clear VCT
    {
        Printf("JCHIP NOT OK!\n");
        return 0;
    }
    i4Ret = DMX_JChipDiag(_u1TsIndex);
    Printf("    Return code: %d\n", i4Ret);
    Printf("%s\n", (i4Ret == 0) ? "JCHIP_OK!" : "JCHIP_FAIL!");

    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}

//-----------------------------------------------------------------------------
/** _DMXCLI_PtsDriftInfo
 *  Command: EnablePtsInfo
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_PtsDriftInfo(INT32 i4Argc, const CHAR** aszArgv)
{

    INT32 i4PtsDrift = 0;
    UINT32 u4PtsDrift = 0;
    UINT32 u4InputCmd = 0;
    if ((i4Argc != 3) && (i4Argc != 2))
    {
        Printf("usage1 : dmx.ptsdrift 0 ---> query current pts drift\n");
        Printf("usage2 : dmx.ptsdrift 1 ptsdrift(90k base) ---> set pts drift value\n");
        Printf("usage2 example : dmx.ptsdrift 1 3000 ---> add pts drift to 3000/90000 = 33ms\n");
        return 0;
    }

    u4InputCmd = StrToInt(aszArgv[1]);

    if (i4Argc == 3)
    {
        if (u4InputCmd == 1)
        {
            u4PtsDrift = StrToInt(aszArgv[2]);
            i4PtsDrift = u4PtsDrift;
            Printf("set pts drift to %d\n",i4PtsDrift);
            STC_SetPtsDrift(0,i4PtsDrift);
        }
    }
    else if (i4Argc == 2)
    {
        i4PtsDrift = STC_GetPtsDrift(0);
        Printf("Current pts drift = %d\n",i4PtsDrift);
    }


    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_EnablePtsInfo
 *  Command: EnablePtsInfo
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_EnablePtsInfo(INT32 i4Argc, const CHAR** aszArgv)
{

    BOOL fgVideoPtsInfo = FALSE;
    BOOL fgAudioPtsInfo = FALSE;
    BOOL fgPcrInfo = FALSE;

    if (i4Argc != 4)
    {
        Printf("usage : dmx.enpts fgVideoPtsInfo fgAudioPtsInfo fgPcrInfo\n");
        Printf("usage exmaple 1 : dmx.enpts 1 0 0 ---> only enable video pts info, you should set dmx.d_l 5 to log pts info\n");
        Printf("usage exmaple 2 : dmx.enpts 0 1 0 ---> only enable audio pts info, you should set dmx.d_l 5 to log pts info\n");
        Printf("usage exmaple 3 : dmx.enpts 0 0 1 ---> only enable pcr value info, you should set dmx.d_l 5 to log pts info\n");
        return 0;
    }

    fgVideoPtsInfo = (BOOL)StrToInt(aszArgv[1]);
    fgAudioPtsInfo = (BOOL)StrToInt(aszArgv[2]);
    fgPcrInfo = (BOOL)StrToInt(aszArgv[3]);

    _DMX_SetPtsInfo(fgVideoPtsInfo,fgAudioPtsInfo);
    _DmxSetPcrInfo(fgPcrInfo);

    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_PrintTraceLogFiles(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i;
    DMX_TRACE_LOGFILE_T trace;

    UNUSED(i4Argc);
    UNUSED(aszArgv);

    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        if (!DMX_CLIGetTraceLogFiles(i, &trace))
        {
            Printf("Error: DMX_CLIGetTraceLogFiles return errors\n");
            break;
        }
        else
        {
            if ((0 == trace.u4CallerAddr) || (0 == trace.u4CalleeAddr) ||
                    (0 == trace.pszCaller) || (0 == trace.pszCallee))
            {
                break;
            }
            else
            {
                Printf("%s(0x%08lx) --> %s(0x%08lx) [Call %lu times, Return %lu times]\n",
                       trace.pszCaller, trace.u4CallerAddr, trace.pszCallee, trace.u4CalleeAddr,
                       trace.u4CallCount, trace.u4ReturnCount);
            }
        }
    }

    if (0 == i)
    {
        Printf("There is nothing to be traced\n");
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_GenerateKey(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4RandNum, u4EncRandNum;
    UINT32 u4SrcPAddr, u4DstPAddr;
    UINT32 i;
    UINT8 *pu1Src, *pu1Dst;
    AES_PARAM_T rAesParam;

    UINT8 au1CtrKey[16] = {0x77, 0x00, 0x51, 0xC1, 0x7E, 0xD4, 0x7A, 0xAE,
                        0x5D, 0x03, 0x2B, 0xC2, 0x04, 0x50, 0xA8, 0x19};

    UINT8 au1CtrIv[16] = {0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
                        0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};

    UINT8 au1SampleKey[16] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 
                        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

    if (i4Argc < 2)
    {
        Printf("%s [random number=0x]\n", aszArgv[0]);
        
        return 0;
    }

    if (i4Argc > 1)
    {
        u4RandNum = (UINT32)StrToInt(aszArgv[1]);
        Printf("Random number: 0x%08x\n", u4RandNum);

        u4SrcPAddr = BSP_AllocAlignedDmaMemory(sizeof(u4RandNum), GCPU_FIFO_ALIGNMENT);
        u4DstPAddr = BSP_AllocAlignedDmaMemory(sizeof(u4RandNum), GCPU_FIFO_ALIGNMENT);
        pu1Src = (UINT8 *)VIRTUAL(u4SrcPAddr); 
        pu1Dst = (UINT8 *)VIRTUAL(u4DstPAddr); 

        x_memcpy(pu1Src, &u4RandNum, sizeof(u4RandNum));
        x_memcpy(rAesParam.au1Key, au1CtrKey, sizeof(au1CtrKey));
        x_memcpy(rAesParam.au1Iv, au1CtrIv, sizeof(au1CtrIv));

        rAesParam.eKeyBitLen = KEY_BIT_LEN_128;
        rAesParam.u4SrcStartAddr = (UINT32)pu1Src;
        rAesParam.u4SrcBufStart = GCPU_LINER_BUFFER_START((UINT32)pu1Src);
        rAesParam.u4SrcBufEnd = GCPU_LINER_BUFFER_END((UINT32)(pu1Src + sizeof(u4RandNum)));
        rAesParam.u4DstStartAddr = (UINT32)pu1Dst;
        rAesParam.u4DstBufStart = GCPU_LINER_BUFFER_START((UINT32)pu1Dst);
        rAesParam.u4DstBufEnd = GCPU_LINER_BUFFER_END((UINT32)(pu1Dst + sizeof(u4RandNum)));
        rAesParam.u4DatLen = sizeof(u4RandNum);
        rAesParam.eMode = BLOCK_CIPHER_MODE_CTR;
        rAesParam.fgEncrypt = TRUE;

        GCPU_Cmd(0, GCPU_AES, &rAesParam);
        u4EncRandNum = *(UINT32 *)pu1Dst;
        
        x_memcpy(au1SampleKey, pu1Dst, sizeof(u4RandNum));
        x_memcpy(au1SampleKey+8, pu1Dst, sizeof(u4RandNum));
        
        VERIFY(BSP_FreeAlignedDmaMemory(u4SrcPAddr));
        VERIFY(BSP_FreeAlignedDmaMemory(u4DstPAddr));
        
        Printf("Encrypted Random number: 0x%08x\n", u4EncRandNum);
        Printf("Sample key: ");
        for (i = 0; i < sizeof(au1SampleKey); i++)
        {
            Printf("0x%02x ", au1SampleKey[i]); 
        }
        Printf("\n");
    }

    return 0;
}

#ifdef CC_DMX_TRACK_ERROR_INTS
static INT32 _DMXCLI_DebugHWError(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Command;

    if (i4Argc < 2)
    {
        Printf("%s [command]\n", aszArgv[0]);
        Printf("    command=0, dump dbm status\n");
        Printf("    command=1, dump error interrupt records\n");
        Printf("    command=2, reset error interrupt records\n");

        return 0;
    }

    if (i4Argc > 1)
    {
        u4Command = (UINT32)StrToInt(aszArgv[1]);
        DMX_CLIHwErrDebugCmd(u4Command);
    }

    return 0;
}
#endif // CC_DMX_TRACK_ERROR_INTS

#ifndef NDEBUG
static void _PrintDebugMask(void)
{
    UINT32 u4Mask, i;

    if (!DMX_LogOperation(DMX_LOG_OP_GETMASK, 0, &u4Mask))
    {
        Printf("Fail to get DMX Log.\n");
        return;
    }

    Printf("\n");

    for (i = 0; i < (sizeof(_arLogMask)/ sizeof(DMX_LOGMASK_T)); i++)
    {
        Printf("%x DMX_LOG_%s: %s\n", i, _arLogMask[i].pu1LogName,
               (_arLogMask[i].u4LogMask & u4Mask)?"Enable": "Disable");
    }

    if (!DMX_LogOperation(DMX_LOG_OP_GETPIDX, 0, &u4Mask))
    {
        Printf("Fail to get DMX Log.\n");
        return;
    }
    Printf("g Monitor Pidx: %s %d\n", (u4Mask != 0xff)?"Enable":"Disable", u4Mask);

    if (!DMX_LogOperation(DMX_LOG_OP_GETSEC, 0, &u4Mask))
    {
        Printf("Fail to get DMX Log.\n");
        return;
    }
    Printf("h Monitor Section Index: %s %d\n", (u4Mask != 0xff)?"Enable":"Disable", u4Mask);

    Printf("Press [Esc] or 'q' to quit the process\n");
}


//-----------------------------------------------------------------------------
/** _DMXCLI_DEBUGLevel
 *  Command: Debug Level setting
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_DebugMask(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1Char, u1Type, u1Value;
    UINT32 u4Mask, u4ToggleBit;

    UNUSED(i4Argc);
    UNUSED(aszArgv);

    if ((i4Argc != 1) && (i4Argc != 3))
    {
        Printf("%s [debug mask:0 / Pidx:1 / SecIdx:2] [value]\n", aszArgv[0]);
        _PrintDebugMask();
        return 1;
    }

    if (i4Argc == 3)
    {
        u1Type = (UINT8)StrToInt(aszArgv[1]);
        u1Value = (UINT8)StrToInt(aszArgv[2]);
        if (u1Type > 2)
        {
            Printf("%s [debug mask:0 / Pidx:1 / Pidx:2] [value]\n", aszArgv[0]);
        }

        if (u1Type == 0)
        {
            if (u1Value > 15)
            {
                Printf("It is a invalid mask value\n");
                return 1;
            }

            if (!DMX_LogOperation(DMX_LOG_OP_GETMASK, 0, &u4Mask))
            {
                Printf("Fail to get DMX Log.\n");
                return 1;
            }

            u4ToggleBit = 1 << (u1Value + 16);
            //Toggle the mask bit
            if (u4Mask & u4ToggleBit)
            {
                // already enable mask, disable it
                u4Mask &= ~u4ToggleBit;
            }
            else
            {
                // enable the bit
                u4Mask |= u4ToggleBit;
            }

            if (!DMX_LogOperation(DMX_LOG_OP_SETMASK, u4Mask, NULL))
            {
                Printf("Fail to set DMX Log.\n");
                return 1;
            }
        }
        else if (u1Type == 1)
        {
            if (!DMX_LogOperation(DMX_LOG_OP_SETPIDX, (UINT32)u1Value, NULL))
            {
                Printf("Fail to set LOG Pidx.\n");
                return 1;
            }
        }
        else if (u1Type == 2)
        {
            if (!DMX_LogOperation(DMX_LOG_OP_SETSEC, (UINT32)u1Value, NULL))
            {
                Printf("Fail to set LOG Section Index.\n");
                return 1;
            }
        }
        else
        {
            Printf("No this type\n");
            return 1;
        }
    }

    // i4Argc == 1, interactive mode
    while (1)
    {
        _PrintDebugMask();
        u1Char = SerPollGetChar();

        if ((u1Char == 27) || (u1Char == 'Q') || (u1Char == 'q')) // escape or q, end of auto pattern
        {
            break;
        }
        if ((u1Char >= '0') && (u1Char <= '9'))
        {
            u1Char = u1Char - 48;
        }
        else if ((u1Char >= 'A')&&(u1Char <= 'H')) // can extend to 'Z'
        {
            u1Char = u1Char - 55;
        }
        else if ((u1Char >= 'a') && (u1Char <= 'h')) // can extend to 'z'
        {
            u1Char = u1Char - 87;
        }
        else
        {
            Printf("Press [Esc] or 'q' to quit the process\n");
            continue;
        }

        if (u1Char < 16) // 0 - 15
        {
            if (!DMX_LogOperation(DMX_LOG_OP_GETMASK, 0, &u4Mask))
            {
                Printf("Fail to get DMX Log.\n");
                return 1;
            }

            u4ToggleBit = 1 << (u1Char + 16);
            //Toggle the mask bit
            if (u4Mask & u4ToggleBit)
            {
                // already enable mask, disable it
                u4Mask &= ~u4ToggleBit;
            }
            else
            {
                // enable the bit
                u4Mask |= u4ToggleBit;
            }

            if (!DMX_LogOperation(DMX_LOG_OP_SETMASK, u4Mask, NULL))
            {
                Printf("Fail to set DMX Log.\n");
                return 1;
            }
        }
        else if ((u1Char == 16) || (u1Char == 17)) // g or h
        {
            UINT8 u1Idx = 0, u1TempChar, i;
            Printf("Set Monitor Index in Hex: (Use FF to disable): ");
            for (i = 0; i < 2; i++)
            {
                u1TempChar = SerPollGetChar();
                if ((u1TempChar >= '0') && (u1TempChar <= '9'))
                {
                    u1TempChar = u1TempChar - 48;
                }
                else if ((u1TempChar >= 'A')&&(u1TempChar <= 'F'))
                {
                    u1TempChar = u1TempChar - 55;
                }
                else if ((u1TempChar >= 'a') && (u1TempChar <= 'f'))
                {
                    u1TempChar = u1TempChar - 87;
                }
                else
                {
                    Printf("Press [Esc] to quit the process\n");
                    continue;
                }

                if (i == 0)
                {
                    u1Idx = (u1TempChar << 4);
                }
                else if (i == 1)
                {
                    u1Idx += u1TempChar;
                }
            }

            if ((u1Char == 16))
            {
                if (!DMX_LogOperation(DMX_LOG_OP_SETPIDX, (UINT32)u1Idx, NULL))
                {
                    Printf("Fail to set LOG Pidx Index.\n");
                    return 1;
                }
            }
            else if (u1Char == 17)
            {
                if (!DMX_LogOperation(DMX_LOG_OP_SETSEC, (UINT32)u1Idx, NULL))
                {
                    Printf("Fail to set LOG Section Index.\n");
                    return 1;
                }
            }
        }
        else
        {
            Printf("No this value\n");
        }
    }

    return 0;
}
#endif //NDEBUG

#ifdef CC_EIT_SECTION_FILTER
static INT32 _DMXCLI_EITFilter(INT32 i4Argc, const CHAR** aszArgv)
{
    CHAR cInputType;
    if(i4Argc < 2)
    {
        Printf("[input_type(p:print EIT filter,e:enable/disable EIT filter,s:Set EIT filter,a:enable/disable EIT filter auto insert)] data...\n");
        return 0;
    }
    cInputType = aszArgv[1][0];
    switch(cInputType)
    {
        case 'p':
            {
                DMX_Print_EIT_Filter();
            }
            break;
        case 'e':
             {
                BOOL fgEnable;
                if(i4Argc < 3)
                {
                    Printf("e 0(disable)/1(enable)\n");
                    return 0;
                }
                fgEnable = (BOOL)StrToInt(aszArgv[2]);
                DMX_Enable_EIT_FILTER(fgEnable);
             }
             break;
         case 's':
            {
                UINT8 u1SetType;
                SECTION_KEY_SET_T tSetEIT;
                if(i4Argc < 3)
                {
                    Printf("s [0:Set section, 1:Clear table 2:clear channel 3:clear all channel]\n");
                    return 0;
                }
                u1SetType = (UINT8)StrToInt(aszArgv[2]);
                switch(u1SetType)
                {
                    case 0:
                       {
                            if(i4Argc < 10)
                            {
                                Printf("s 0 onid svcid tsid secnum tableid version [1(Set)/0(Clear)]\n");
                                return 0;
                            }
                            tSetEIT.eSetType =  EIT_SET_SECTION;
                            tSetEIT.tSecKey.e_key.ui2_onid = (UINT16)StrToInt(aszArgv[3]);
                            tSetEIT.tSecKey.e_key.ui2_svcid = (UINT16)StrToInt(aszArgv[4]);
                            tSetEIT.tSecKey.e_key.ui2_tsid = (UINT16)StrToInt(aszArgv[5]);
                            tSetEIT.tSecKey.ui1_secnum = (UINT8)StrToInt(aszArgv[6]);
                            tSetEIT.tSecKey.ui1_tableid = (UINT8)StrToInt(aszArgv[7]);
                            tSetEIT.tSecKey.ui1_version= (UINT8)StrToInt(aszArgv[8]);
                            tSetEIT.fgSet = (UINT8)StrToInt(aszArgv[9]);
                            DMX_SetEITSection(&tSetEIT);
                       }
                       break;
                    case 1:
                       {
                            if(i4Argc < 7)
                            {
                                Printf("s 1 onid svcid tsid tableid\n");
                                return 0;
                            }
                            tSetEIT.eSetType =  EIT_SET_TABLE;
                            tSetEIT.tSecKey.e_key.ui2_onid = (UINT16)StrToInt(aszArgv[3]);
                            tSetEIT.tSecKey.e_key.ui2_svcid = (UINT16)StrToInt(aszArgv[4]);
                            tSetEIT.tSecKey.e_key.ui2_tsid = (UINT16)StrToInt(aszArgv[5]);
                            tSetEIT.tSecKey.ui1_tableid = (UINT8)StrToInt(aszArgv[6]);
                            DMX_SetEITSection(&tSetEIT);
                       }
                       break;
                    case 2:
                       {
                            if(i4Argc < 6)
                            {
                                Printf("s 2 onid svcid tsid\n");
                                return 0;
                            }
                            tSetEIT.eSetType =  EIT_SET_CHANNEL;
                            tSetEIT.tSecKey.e_key.ui2_onid = (UINT16)StrToInt(aszArgv[3]);
                            tSetEIT.tSecKey.e_key.ui2_svcid = (UINT16)StrToInt(aszArgv[4]);
                            tSetEIT.tSecKey.e_key.ui2_tsid = (UINT16)StrToInt(aszArgv[5]);
                            DMX_SetEITSection(&tSetEIT);
                       }
                       break;
                    case 3:
                       {
                            tSetEIT.eSetType =  EIT_SET_ALL_CHANNEL;
                            DMX_SetEITSection(&tSetEIT);
                       }
                       break;
                    default:
                        Printf("s [0:Set section, 1:Clear table 2:clear channel 3:clear all channel]\n");
                        break;
                }
            }
            break;
            case 'a':
                {
                    BOOL fgEnable;
                    if(i4Argc < 3)
                    {
                        Printf("1 0(disable)/1(enable)\n");
                        return 0;
                    }
                    fgEnable = (BOOL)StrToInt(aszArgv[2]);
                    DMX_ENABLE_EIT_Filter_Auto_Insert(fgEnable);
                }
            break;
            default:
                Printf("[input_type(p:print EIT filter,e:enable/disable EIT filter,s:Set EIT filter,a:enable/disable EIT filter auto insert)] data...\n");
                break;
    }
    return 0;
}
#endif


//-----------------------------------------------------------------------------
/** _DMXCLI_Init
 *  Command: init
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_Init(INT32 i4Argc, const CHAR** aszArgv)
{
    if (!DMX_Init())
    {
        Printf("DMX_Init fail\n");
        return -1;
    }

    Printf("DMX_Init OK\n");

    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}

//-----------------------------------------------------------------------------
/** _DMXCLI_SetFrontEnd
 *  Command: init
 */
//-----------------------------------------------------------------------------
static void _ShowSetFrontEndUsage(void)
{
#if defined(CC_MT5890)
    if(IS_IC_5861())
    {
        Printf( "-------------FrontEnd Usage-------------\n"
            "DMX_FE_INTERNAL = 0,\n"
            "DMX_FE_DDI = 2,\n"
            "DMX_FE_TSOUT = 3,\n"
            "DMX_FE_CI = 4,\n"
            "DMX_FE_EXT_S = 0x10,\n"
            "DMX_FE_EXT_P = 0x11,\n"
            "DMX_FE_EXT_2BIT = 0x12,\n"
            "DMX_FE_EXT2_0_S = 0x20,\n"
            "DMX_FE_EXT2_0_P = 0x21,\n"
            "DMX_FE_EXT2_0_2BIT = 0x22,\n"
            "DMX_FE_EXT2_1_S = 0x30,\n"
            "DMX_FE_EXT2_2_S = 0x40,\n"
            "DMX_FE_EXT2_3_S = 0x50,\n"
            "DMX_FE_EXT2_4_S = 0x60,\n"
            "DMX_FE_EXT3_0_S = 0x70,\n"
            "DMX_FE_EXT3_1_S = 0x80,\n"
            "DMX_FE_EXT3_2_S = 0x90,\n"
            "DMX_FE_EXT4_S = 0xA0,\n"
          );
    }
    else
    {
        Printf( "-------------FrontEnd Usage-------------\n"
            "DMX_FE_INTERNAL = 0,\n"
            "DMX_FE_DDI = 2,\n"
            "DMX_FE_TSOUT = 3,\n"
            "DMX_FE_CI = 4,\n"
            "DMX_FE_EXT_S = 0x10,\n"
            "DMX_FE_EXT_P = 0x11,\n"
            "DMX_FE_EXT_2BIT = 0x12,\n"
            "DMX_FE_EXT2_0_S = 0x20,\n"
            "DMX_FE_EXT2_0_P = 0x21,\n"
            "DMX_FE_EXT2_0_2BIT = 0x22,\n"
            "DMX_FE_EXT2_1_S = 0x30,\n"
            "DMX_FE_EXT2_2_S = 0x40,\n"
            "DMX_FE_EXT2_3_S = 0x50,\n"
            "DMX_FE_EXT2_4_S = 0x60,\n"
            "DMX_FE_EXT3_0_S = 0x70,\n"
            "DMX_FE_EXT3_1_S = 0x80,\n"
            "DMX_FE_EXT4_S = 0xA0,\n"
          );
    }
#elif defined(CC_MT5882)
    Printf( "-------------FrontEnd Usage-------------\n"
            "DMX_FE_INTERNAL = 0.\n"
            "DMX_FE_DDI = 2,\n"
            "DMX_FE_TSOUT = 3,\n"
            "DMX_FE_CI = 4,\n"
            "DMX_FE_EXT_S = 0x10,\n"
            "DMX_FE_EXT_P = 0x11,\n"
            "DMX_FE_EXT_2BIT = 0x12,\n"
            "DMX_FE_EXT_1_S = 0x20,\n"
            "DMX_FE_EXT_1_P = 0x21,\n"
            "DMX_FE_EXT_1_2BIT = 0x22,\n"
            "DMX_FE_EXT2_0_S = 0x30,\n"
            "DMX_FE_EXT2_0_P = 0x31,\n"
            "DMX_FE_EXT2_0_2BIT = 0x32,\n"
            "DMX_FE_EXT2_1_S = 0x40,\n"
            "DMX_FE_EXT2_2_S = 0x50,\n"
            "DMX_FE_EXT2_3_S = 0x60,\n"
            "DMX_FE_EXT3_S = 0x70,\n"
            "DMX_FE_EXT4_S = 0x80,\n"
          );
#endif
}

static INT32 _DMXCLI_SetFrontEnd(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1TsIndex = 0;
    DMX_FRONTEND_T eFEType;

    if (i4Argc < 2)
    {
        Printf("%s fe_type ts_idx\n", aszArgv[0]);
        Printf("fe_type: 0, 1, ...\n");

        _ShowSetFrontEndUsage();

        Printf("---------Current FrontEnd States-------\n");
        Printf("Current FE type: tsidx 0, 0x%x\n", (UINT32)DMX_GetFrontEndEx(0));
        Printf("Current FE type: tsidx 1, 0x%x\n", (UINT32)DMX_GetFrontEndEx(1));

#if defined(CC_MT5882) 
        Printf("Current FE type: tsidx 3, 0x%x\n", (UINT32)DMX_GetFrontEndEx(3));
#elif defined(CC_MT5890)
        Printf("Current FE type: tsidx 2, 0x%x\n", (UINT32)DMX_GetFrontEndEx(2));
        Printf("Current FE type: tsidx 3, 0x%x\n", (UINT32)DMX_GetFrontEndEx(3));
#endif

        return 0;
    }

    eFEType = (DMX_FRONTEND_T)StrToInt(aszArgv[1]);
    u1TsIndex = StrToInt(aszArgv[2]);

#if defined(CC_MT5882)
    if (u1TsIndex == 0x2)
    {
        Printf("Please set the right tsindex.(0,1,3)\n");
        return -1;
    }
#endif

    if (!DMX_SetFrontEndEx(u1TsIndex, eFEType))
    {
        Printf("Set front end fail\n");
        return -1;
    }

//    _u1TsIndex = DMX_GetFramerIndex();

    Printf("OK\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_ResetFramer
 *  Command: reset framer
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_ResetFramer(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1FramerIdx = 0;

    if (i4Argc < 2)
    {
        Printf("%s FramerIdx\n", aszArgv[0]);
        return 0;
    }

    u1FramerIdx = StrToInt(aszArgv[1]);

    DMX_ResetFramer(u1FramerIdx);

    return 0;
}


//-----------------------------------------------------------------------------
static void _DmxCLI_FramerUsage(const CHAR* szArgv)
{
    Printf("%s [frameridx][-p 0|1] [-e 0|1] [-s 0|1]\n", szArgv);
    Printf("frameridx:  0=framer0, 1=framer1, 2=framer2, 3=frmaer3.\n");
    Printf("    -p   :  0=serial, 1=parallel.\n");
    Printf("    -e   :  0=neg_edge, 1=pos_edge.\n");
    Printf("    -s   :  0=inernal_sync, 1=ext_sync.\n");
}

//-----------------------------------------------------------------------------
/** _DMXCLI_SetFramer
 *  Command: set framer mode
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetFramer(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i;
    UINT32 u4FramerIdx, u4Sync, u4Edge, u4Parallel, u4Val, u4Ctrl;

#if defined(CC_MT5399) || defined(CC_MT5890)
    UINT32 u4Reg;
#endif

    if (i4Argc < 2)
    {
        _DmxCLI_FramerUsage(aszArgv[0]);
        Printf("-------------Current Framer States------------\n");
        u4Ctrl = IO_READ32(DEMUX0_BASE, 0x20);

        //Framer 0
        Printf("Framer0: %s, %s, %s, %s\n",
               (u4Ctrl & 0x1) ? "Enable" : "Disable",
               (u4Ctrl & 0x2) ? "Parallel" : "Serial",
               (u4Ctrl & 0x4) ? "External sync" : "Internal sync",
               (u4Ctrl & 0x8) ? "Positive" : "Negative");

        //Framer 1
        Printf("Framer1: %s, %s, %s, %s\n",
               (u4Ctrl & 0x10) ? "Enable" : "Disable",
               (u4Ctrl & 0x20) ? "Parallel" : "Serial",
               (u4Ctrl & 0x40) ? "External sync" : "Internal sync",
               (u4Ctrl & 0x80) ? "Positive" : "Negative");
#if defined(CC_MT5399) || defined(CC_MT5890)
        //Framer 2
        u4Reg = IO_READ32(DEMUX0_BASE, 0x924);
        Printf("Framer2: %s, %s, %s, %s\n",
               (u4Reg & 0x1) ? "Enable" : "Disable",
               (u4Reg & 0x2) ? "Parallel" : "Serial",
               (u4Reg & 0x4) ? "External sync" : "Internal sync",
               (u4Reg & 0x8) ? "Positive" : "Negative");
#endif

#if defined(CC_MT5399) || defined(CC_MT5398)|| defined(CC_MT5890)
        //Framer 3
        Printf("Framer3: %s, %s, %s, %s\n",
               (u4Ctrl & 0x100) ? "Enable" : "Disable",
               (u4Ctrl & 0x200) ? "Parallel" : "Serial",
               (u4Ctrl & 0x400) ? "External sync" : "Internal sync",
               (u4Ctrl & 0x800) ? "Positive" : "Negative");
#endif

        Printf("----------------------------------------\n");

        return 0;
    }

    u4FramerIdx = StrToInt(aszArgv[1]);

#if !defined(CC_MT5399) || !defined(CC_MT5890)
    if (u4FramerIdx == 2)
    {
        Printf("Tsindex 2 Error!\n");
    }
#endif

    if (u4FramerIdx == 3)
    {
        u4Val = (IO_READ32(DEMUX0_BASE, 0x20) & 0xFFF) >> ((u4FramerIdx-1) * 4);
    }
    else if (u4FramerIdx == 2)
    {
        u4Val = IO_READ32(DEMUX0_BASE, 0x924) & 0xF;
    }
    else
    {
        u4Val = (IO_READ32(DEMUX0_BASE, 0x20) & 0xFFF) >> (u4FramerIdx * 4);
    }

    u4Parallel = (u4Val >> 1) & 0x1;
    u4Sync = (u4Val >> 2) & 0x1;
    u4Edge = (u4Val >> 3) & 0x1;

    for (i=2; i<(i4Argc-1); i++)
    {
        if (aszArgv[i][0] != '-')
        {
            continue;
        }
        switch (aszArgv[i][1])
        {
        case 'P':
        case 'p':
            u4Parallel = (StrToInt(aszArgv[i+1])) & 0x1;
            break;
        case 'E':
        case 'e':
            u4Edge = (StrToInt(aszArgv[i+1])) & 0x1;
            break;
        case 'S':
        case 's':
            u4Sync = (StrToInt(aszArgv[i+1])) & 0x1;
            break;
        default:
            break;
        }
    }

    if (u4FramerIdx == 0)
    {
        u4Val = IO_READ32(DEMUX0_BASE, 0x20) & 0xFFFFFFF0;
    }
    else if (u4FramerIdx == 1)
    {
        u4Val = IO_READ32(DEMUX0_BASE, 0x20) & 0xFFFFFF0F;
    }
#if defined(CC_MT5399) || defined(CC_MT5890)
    else if (u4FramerIdx == 2)
    {
        u4Val = IO_READ32(DEMUX0_BASE, 0x924) & 0xFFFFFFF0;
    }
#endif
#if defined(CC_MT5398) || defined(CC_MT5399)|| defined(CC_MT5890)
    else if (u4FramerIdx == 3)
    {
        u4Val = IO_READ32(DEMUX0_BASE, 0x20) & 0xFFFFF0FF;
    }
#endif
    else
    {
        Printf("Please type right framer index!(0,1,2,3).\n");
    }

    if (u4FramerIdx == 3)
    {
        u4Val |= (1 | (u4Parallel << 1) | (u4Sync << 2) | (u4Edge << 3)) << (4 * (u4FramerIdx - 1));
    }
    else if (u4FramerIdx == 2)
    {
        u4Val |= (1 | (u4Parallel << 1) | (u4Sync << 2) | (u4Edge << 3));
    }
    else
    {
        u4Val |= (1 | (u4Parallel << 1) | (u4Sync << 2) | (u4Edge << 3)) << (4 * u4FramerIdx);
    }

    if (u4FramerIdx == 2)
    {
        IO_WRITE32(DEMUX0_BASE, 0x924, u4Val);
    }
    else
    {
        IO_WRITE32(DEMUX0_BASE, 0x20, u4Val);
    }

    DMX_ResetFramer(u4FramerIdx);

    Printf("OK\n");

    return 0;
}

#if defined(CC_MT5399) || defined(CC_MT5890)

//-----------------------------------------------------------------------------
/** _DMXCLI_SetFramer
 *  Command: set framer mode
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetFramerPacketErrorHandling(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1Tsidx;
    UINT32 u4Ctrl, u4Val = 0xF0C0;

    if (i4Argc < 2)
    {
        Printf("%s,ts_idx,ErrorTable.\n", aszArgv[0]);
        Printf("-------------Current Framer ErrorHandling------------\n");
        //framer 0
        u4Ctrl = DMX_GetFramerPacketErrorHandling(0);
        Printf("Framer0: 0x%x.\n",u4Ctrl);

        //framer 1
        u4Ctrl = DMX_GetFramerPacketErrorHandling(1);
        Printf("Framer1: 0x%x.\n",u4Ctrl);


#if defined(CC_MT5399) || defined(CC_MT5890)

        //framer 2
        u4Ctrl = DMX_GetFramerPacketErrorHandling(2);
        Printf("Framer2: 0x%x.\n",u4Ctrl);
#endif
        //framer 3
        u4Ctrl = DMX_GetFramerPacketErrorHandling(3);
        Printf("Framer3: 0x%x.\n",u4Ctrl);
        Printf("----------------------------------------\n");

        return 0;
    }

    u1Tsidx = (UINT8)StrToInt(aszArgv[1]);

    if (i4Argc > 2)
    {
        u4Val = StrToInt(aszArgv[2]);
    }

    DMX_SetFramerPacketErrorHandling(u1Tsidx, TRUE, u4Val);

    Printf("OK\n");

    return 0;
}

static void _PrintInputSourceInfo(UINT8 u1TsIdx, const DMX_DBM_INPUT_SOURCE_T eSource)
{
    switch (eSource)
    {
    case DMX_DBM_INPUT_DDI:
        Printf("Tsidex: %d, InputSource: DDI.\n",u1TsIdx);
        break;

    case DMX_DBM_INPUT_FRAMER:
        Printf("Tsidex: %d, InputSource: FRAMER.\n",u1TsIdx);
        break;

    case DMX_DBM_INPUT_PB:
        Printf("Tsidex: %d, InputSource: PB.\n",u1TsIdx);
        break;

    case DMX_DBM_INPUT_PB2:
        Printf("Tsidex: %d, InputSource: PB2.\n",u1TsIdx);
        break;

    default:
        break;
    }
}


//-----------------------------------------------------------------------------
/** _DMXCLI_SetDbmInputSource
 *  Command: set framer mode
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetDbmInputSource(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1Tsidx;
    DMX_DBM_INPUT_SOURCE_T eSource;

    if (i4Argc < 2)
    {
        Printf("%s,ts_idx,inputsource.\n", aszArgv[0]);
        Printf("ts_idx: 2, 3.\n");
        Printf("inputsource: DDI: 0, Framer : 1, PB: 2, PB2: 3.\n");
        Printf("-------------Current Dbm Input Source------------\n");
        //framer 2
        eSource = DMX_GetDbm_InputSource(2);
        _PrintInputSourceInfo(2,eSource);

        //framer 3
        eSource = DMX_GetDbm_InputSource(3);
        _PrintInputSourceInfo(3,eSource);

        Printf("----------------------------------------\n");
        return 0;
    }

    u1Tsidx = (UINT8)StrToInt(aszArgv[1]);

    eSource = (DMX_DBM_INPUT_SOURCE_T)StrToInt(aszArgv[2]);

    DMX_SetDbm_InputSource(u1Tsidx, eSource);

    Printf("OK\n");

    return 0;
}
#endif

//-----------------------------------------------------------------------------
/** Function _DMX_SetTsIndex(UINT8 u1TunerIdx)
 *  Command: Set ts index
 */
//-----------------------------------------------------------------------------

static INT32 _DMXCLI_SetTsIndex(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1TsIndex;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 2)
    {
        Printf("Usage: %s Ts index[0,1,3]\n",aszArgv[0]);
        return 0;
    }

    u1TsIndex = (UINT8)StrToInt(aszArgv[1]);
    if (u1TsIndex > DMX_MAX_TUNER_INDEX)
    {
        return -1;
    }

    if (!DMX_SetTsIndex(u1TsIndex))
    {
        Printf("DMX set TS index fail!\n");
        return -1;
    }
    Printf("DMX set TS index OK!\n");
    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Error code
 */
//-----------------------------------------------------------------------------
static BOOL _RawTSHandler(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
                          UINT32 u4Data, const void* pvNotifyTag)
{
    /* just fill the buffer */

    UNUSED(u1Pidx);
    UNUSED(eCode);
    UNUSED(u4Data);
    UNUSED(pvNotifyTag);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _ShowSetPidUsage(const CHAR* szCmd)
{
    Printf("Usage: %s pid_index(0-%u) pid [pid_type(pes,video,audio,none]"
           " [device_id(0..x)] [file(0|1)] [ca_mode(0-%u)] [key_index(0-%u)]\n",
           szCmd, DMX_NUM_PID_INDEX - 1, DMX_MAX_CA_MODE, DMX_DMEM_CA_KEY_NUM - 1);
}

#ifndef NDEBUG
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static BOOL _MwMPEGVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rMpvPes;

    ASSERT(prPes != NULL);

    x_memset((void*)&rMpvPes, 0, sizeof(VDEC_PES_INFO_T));

    switch (prPes->u4FrameType)
    {
    case PIC_TYPE_I:
        rMpvPes.ucPicType = MPV_I_TYPE;
        break;

    case PIC_TYPE_P:
        rMpvPes.ucPicType = MPV_P_TYPE;
        break;

    case PIC_TYPE_B:
        rMpvPes.ucPicType = MPV_B_TYPE;
        break;

    default:
        return TRUE;
    }

    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;
    rMpvPes.u4VldReadPtr = prPes->u4FrameAddr;
    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    rMpvPes.fgGop = prPes->fgGop;
    rMpvPes.fgEos = prPes->fgEOS;

    if (prPes->fgEOS)
    {
        Printf("MPEG-2 VideoCallback EOS.\n");
    }

    return (VDEC_SendEs(&rMpvPes) == 1);
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static BOOL _MwH264VideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T  rMpvPes;
    UINT32 u4Addr;

    ASSERT(prPes != NULL);

    x_memset((void*)&rMpvPes, 0, sizeof(VDEC_PES_INFO_T));

    if (!H264_PIC_VALID_TYPE(prPes->u4FrameType))
    {
        return TRUE;
    }
    rMpvPes.ucPicType = prPes->au1PicInfo[0];
    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;

    u4Addr = prPes->u4FrameAddr + 4;
    if (u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }
    rMpvPes.u4VldReadPtr = u4Addr;

    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    //rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    //rMpvPes.fgGop = prPes->fgGop;

    rMpvPes.fgEos = prPes->fgEOS;
    rMpvPes.u1SliceCount = prPes->u1SliceCount;

    if (prPes->fgEOS)
    {
        Printf("h264 VideoCallback EOS.\n");
    }

    return (VDEC_SendEs((void *)&rMpvPes) == 1);
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static BOOL _AVSVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T  rMpvPes;
    UINT32 u4Addr;

    ASSERT(prPes != NULL);

    x_memset((void*)&rMpvPes, 0, sizeof(rMpvPes));

#if defined(CC_TRUSTZONE_SUPPORT) && defined(CC_SVP_SUPPORT)
    u4Addr = (UINT32)prPes->au1PicInfo;
#else
    u4Addr = prPes->u4FrameAddr + 3;
    if (u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }
#endif
    rMpvPes.ucPicType = *(UINT8*)VIRTUAL(u4Addr);

    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;

    rMpvPes.u4VldReadPtr = prPes->u4FrameAddr;

    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    //rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    //rMpvPes.fgGop = prPes->fgGop;

    rMpvPes.fgEos = prPes->fgEOS;

    if (prPes->fgEOS)
    {
        Printf("AVS VideoCallback EOS.\n");
    }

    return (VDEC_SendEs((void *)&rMpvPes) == 1);
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxVideoCallback(const DMX_PES_MSG_T* prPes)
{

    switch (prPes->eVideoType)
    {
    case DMX_VIDEO_MPEG:
        return _MwMPEGVideoCallback(prPes);

    case DMX_VIDEO_H264:
        return _MwH264VideoCallback(prPes);

    case DMX_VIDEO_AVS:
        return _AVSVideoCallback(prPes);

    default:
        Printf("Unknown video type\n");
        break;
    }
    return TRUE;
}
#endif//#ifndef NDEBUG
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetPid(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PID_T rPid;
    UINT8 u1Pidx, u1File = 0;
    BOOL fgRet;

    ASSERT(aszArgv != NULL);

    if (i4Argc < 3)
    {
        _ShowSetPidUsage(aszArgv[0]);
        return -1;
    }

    x_memset(&rPid, 0, sizeof(rPid));

    rPid.u1TsIndex = _u1TsIndex;
    rPid.u1DeviceId = 0;
    rPid.u1KeyIndex = 0;
    rPid.fgEnable = TRUE;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.fgAllocateBuffer = TRUE;
    rPid.ePcrMode = DMX_PCR_MODE_NONE;
    rPid.ePidType = DMX_PID_TYPE_ES_OTHER;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = DMX_DEFAULT_ES_FIFO_SIZE;
    rPid.pfnNotify = NULL;
    rPid.pvNotifyTag = NULL;
    rPid.fgPrimary = FALSE;
    rPid.u1ChannelId = 0x0;
    rPid.fgDisableFifoOutput = FALSE;

    u1Pidx = (UINT8)StrToInt(aszArgv[1]);
    rPid.u2Pid = (UINT16)StrToInt(aszArgv[2]);

    if (i4Argc > 3)
    {
        if (x_strcmp(aszArgv[3], "pes") == 0)
        {
            rPid.ePidType = DMX_PID_TYPE_PES;
        }
        else if (x_strcmp(aszArgv[3], "video") == 0)
        {
            rPid.ePidType = DMX_PID_TYPE_ES_VIDEO;
        }
        else if (x_strcmp(aszArgv[3], "audio") == 0)
        {
            rPid.ePidType = DMX_PID_TYPE_ES_AUDIO;
        }
        else if (x_strcmp(aszArgv[3], "raw") == 0)
        {
            rPid.ePidType = DMX_PID_TYPE_TS_RAW;
        }
        else if (x_strcmp(aszArgv[3], "none") == 0)
        {
            rPid.ePidType = DMX_PID_TYPE_NONE;
        }
        else
        {
            _ShowSetPidUsage(aszArgv[0]);
            return -1;
        }
    }

    if (i4Argc > 4)
    {
        rPid.u1DeviceId = (UINT8)StrToInt(aszArgv[4]);
    }

    if (i4Argc > 5)
    {
        u1File = (UINT8)StrToInt(aszArgv[5]);
    }

    if (i4Argc > 6)
    {
        rPid.eDescMode = (DMX_DESC_MODE_T)(UINT8)StrToInt(aszArgv[6]);
    }

    if (i4Argc > 7)
    {
        rPid.u1KeyIndex = (UINT8)StrToInt(aszArgv[7]);
    }

    if (i4Argc > 8)
    {
        rPid.fgPrimary = (UINT8)StrToInt(aszArgv[8]);
    }

    if (i4Argc > 9)
    {
        _ShowSetPidUsage(aszArgv[9]);
        return -1;
    }
    
#ifndef NDEBUG
    if(u1File == 0)
    { 
        UINT8 u1Inst;
        DMX_DECODER_CALLBACKS_T rDecoderCallbacks;
        // set the framer 0 pid for tuner 0
        u1Inst = DMX_MUL_GetAvailableInst(DMX_CONN_TYPE_TUNER_0);

        u1Pidx = DMX_MUL_GetAvailablePidx(u1Inst);
        DMX_MUL_SetInstType(u1Inst, DMX_IN_BROADCAST_TS);
        DMX_MUL_SetTSIdx(u1Inst);
        
        //set dtv av decoder callback;
        rDecoderCallbacks.pfnSendVideoPes = _MwDmxVideoCallback;
        rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
        rDecoderCallbacks.pfnSendAudioPes = AUD_SendAudioPes;
        DMX_MUL_SetDecoderCallbacks(u1Inst, &rDecoderCallbacks);
    }
#endif
    
    // Process audio FIFO specially
    if (rPid.ePidType == DMX_PID_TYPE_ES_AUDIO)
    {
        UINT32 au4BufStart[2], au4BufEnd[2];
#ifndef CC_MT5391_AUD_3_DECODER
        ASSERT(NUM_AUDIO_DEVICE == 2);
#else
        ASSERT(NUM_AUDIO_DEVICE >= 3);
#endif  // CC_MT5391_AUD_3_DECODER
        if (rPid.u1DeviceId >= NUM_AUDIO_DEVICE)
        {
            Printf("Audio device ID: 0 - %u\n", NUM_AUDIO_DEVICE - 1);
            return -1;
        }

#ifdef AUD_OLDFIFO_INTF
        if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
                           &au4BufEnd[1]) != AUD_OK)
#else
        if ((AUD_GetAudFifo(0, &au4BufStart[0], &au4BufEnd[0]) != AUD_OK) ||
                (AUD_GetAudFifo(1, &au4BufStart[1], &au4BufEnd[1]) != AUD_OK))
#endif
        {
            Printf("Can't get audio buffers!\n");
            return -1;
        }
        if ((au4BufStart[0] == 0) || (au4BufStart[1] == 0))
        {
            Printf("Can't get audio buffers!\n");
            return -1;
        }
#ifndef CC_MT5391_AUD_3_DECODER
        rPid.u4BufAddr = au4BufStart[rPid.u1DeviceId];
        rPid.u4BufSize = au4BufEnd[rPid.u1DeviceId] -
                         au4BufStart[rPid.u1DeviceId];
#else
        //gallen, decoder 1/3 both use AFIFO1, decoder 2 uses AFIFO2
        rPid.u4BufAddr = au4BufStart[(((rPid.u1DeviceId)==1)?1:0)];
        rPid.u4BufSize = au4BufEnd[(((rPid.u1DeviceId)==1)?1:0)] -
                         au4BufStart[(((rPid.u1DeviceId)==1)?1:0)];
#endif  // CC_MT5391_AUD_3_DECODER
        rPid.fgAllocateBuffer = FALSE;
    }
    else if (rPid.ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
#if 1
        UINT32 u4Addr, u4Size;

        // Allocate video buffer from FBM
#if defined(CC_TRUSTZONE_SUPPORT) && defined(CC_SVP_SUPPORT)
        _prDmxFbmPool = FBM_Alloc(FBM_TYPE_TZ_DMX);
#else
        _prDmxFbmPool = FBM_Alloc(FBM_TYPE_DMX);
#endif
        ASSERT(_prDmxFbmPool != NULL);
        ASSERT(_prDmxFbmPool->u4Addr != 0);
        u4Addr = _prDmxFbmPool->u4Addr;
        u4Size = _prDmxFbmPool->u4Size;
        LOG(0, "FBM DMX addr=0x%08x, size=0x%x\n", u4Addr, u4Size);
        
        // Setup buffer attributes
        rPid.fgAllocateBuffer = FALSE;
        rPid.u4BufAddr = u4Addr;
        rPid.u4BufSize = u4Size;
#else
        rPid.fgAllocateBuffer = TRUE;
        rPid.u4BufSize = 0x0500000;
#endif  // 1
    }
    else if (rPid.ePidType == DMX_PID_TYPE_TS_RAW)
    {
        rPid.pfnNotify = _RawTSHandler;
    }

    fgRet = DMX_SetPidEx(u1Pidx, DMX_PID_FLAG_ALL, &rPid,
                         (u1File == 0) ? DMX_SOURCE_DTV0 : DMX_SOURCE_TSFILE0);

    return fgRet ? 0 : -1;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_FreePid(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1Pidx;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 2)
    {
        Printf("Usage: %s pid_index(0-%u)\n", aszArgv[0],
               DMX_NUM_PID_INDEX - 1);
        return -1;
    }

    u1Pidx = (UINT8)StrToInt(aszArgv[1]);
    if (!DMX_FreePid(u1Pidx))
    {
        return -1;
    }

    FBM_Free(_prDmxFbmPool);
    _prDmxFbmPool = NULL;

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_GetPid(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PID_T rPid;
    UINT32 i;
    UINT8 u1Pidx;

    ASSERT(aszArgv != NULL);

    if (i4Argc > 2)
    {
        Printf("Usage: %s [all|pid_index(0-%u)]\n", aszArgv[0],
               DMX_NUM_PID_INDEX - 1);
        return -1;
    }

    if ((i4Argc == 1) || (x_strcmp(aszArgv[1], "all") == 0))
    {
        // Show infos of all active PIDs
        for (i = 0; i < DMX_NUM_PID_INDEX; i++)
        {
            if (!DMX_CLIGetPid(i, DMX_PID_FLAG_ALL, &rPid))
            {
                return -1;
            }

            if (rPid.fgEnable)
            {
                _ShowPidInfo(i, &rPid);
            }
        }
    }
    else
    {
        // Show single PID info
        u1Pidx = (UINT8)StrToInt(aszArgv[1]);
        if (!DMX_CLIGetPid(u1Pidx, DMX_PID_FLAG_ALL, &rPid))
        {
            return -1;
        }
        _ShowPidInfo(u1Pidx, &rPid);
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_GetPidMaxSize(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4PsiMaxSize = 0,u4PesMaxSize =0;

    ASSERT(aszArgv != NULL);

    if (i4Argc > 2)
    {
        Printf("Usage: %s \n", aszArgv[0]);
        return -1;
    }

    if (i4Argc == 1)
    {
        u4PsiMaxSize = DMX_CLIGetPsiMaxSize();
        u4PesMaxSize = DMX_CLIGetPesMaxSize();
        Printf("\nThe PSI max memory size is:0X%x\n",u4PsiMaxSize);
        Printf("\nThe PES max memory size is:0X%x\n",u4PesMaxSize);
    }
    else if (i4Argc == 2)
    {
        if (x_strcmp(aszArgv[1], "psi") == 0)
        {
            u4PsiMaxSize = DMX_CLIGetPsiMaxSize();
            Printf("\nThe PSI max memory size is:0X%x\n",u4PsiMaxSize);
        }
        else if (x_strcmp(aszArgv[1], "pes") == 0)
        {
            u4PesMaxSize = DMX_CLIGetPesMaxSize();
            Printf("\nThe PES max memory size is:0X%x\n",u4PesMaxSize);
        }
    }
    else
    {
        return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_EnablePid(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PID_T rPid;
    UINT8 u1Pidx;

    ASSERT(aszArgv != NULL);

    if ((i4Argc != 2) && (i4Argc != 3))
    {
        Printf("Usage: %s pid_index [on|off]\n", aszArgv[0]);
        return -1;
    }

    u1Pidx = (UINT8)StrToInt(aszArgv[1]);
    if (i4Argc == 2)
    {
        if (!DMX_CLIGetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
        {
            return -1;
        }

        Printf("Pid_index %u %s\n", u1Pidx,
               rPid.fgEnable ? "enabled" : "disabled");

        return 0;
    }

    if (x_strcmp(aszArgv[2], "on") == 0)
    {
        rPid.fgEnable = TRUE;
    }
    else if (x_strcmp(aszArgv[2], "off") == 0)
    {
        rPid.fgEnable = FALSE;
    }
    else
    {
        return -1;
    }

    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
    {
        return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _ShowPcrUsage(const CHAR* szCmd)
{
    Printf("Usage: %s pid_index(0-%u) [mode(2|0)] [on|off]\n", szCmd,
           DMX_NUM_PID_INDEX - 1);
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetPcr(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1PidIndex;
    BOOL fgEnable = TRUE, fgNewMode = TRUE;

    ASSERT(aszArgv != NULL);

    if (i4Argc < 2)
    {
        _ShowPcrUsage(aszArgv[0]);
        return -1;
    }

    u1PidIndex = (UINT8)StrToInt(aszArgv[1]);
    if (u1PidIndex >= DMX_NUM_PID_INDEX)
    {
        _ShowPcrUsage(aszArgv[0]);
        return -1;
    }

    if (i4Argc > 2)
    {
        UINT8 u1Mode;

        u1Mode = (UINT8)StrToInt(aszArgv[2]);
        if ((u1Mode != 0) && (u1Mode != 2))
        {
            _ShowPcrUsage(aszArgv[0]);
            return -1;
        }

        fgNewMode = (u1Mode == 2);
    }

    if (i4Argc > 3)
    {
        if (x_strcmp(aszArgv[3], "on") == 0)
        {
            fgEnable = TRUE;
        }
        else if (x_strcmp(aszArgv[3], "off") == 0)
        {
            fgEnable = FALSE;
        }
        else
        {
            _ShowPcrUsage(aszArgv[0]);
            return -1;
        }
    }

    if (!DMX_SetPcr(u1PidIndex, fgEnable, fgNewMode))
    {
        return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetPsiPid(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PID_T rPid;
    UINT16 u2Pid;
    UINT8 u1PidIndex;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 3)
    {
        Printf("Usage: %s pid_index pid\n", aszArgv[0]);
        return -1;
    }

    u1PidIndex = (UINT8)StrToInt(aszArgv[1]);
    u2Pid = (UINT16)StrToInt(aszArgv[2]);

    rPid.u1TsIndex = _u1TsIndex;
    rPid.u1DeviceId = 0;
    rPid.u1KeyIndex = 0;
    rPid.fgEnable = TRUE;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.fgAllocateBuffer = TRUE;
    rPid.ePcrMode = DMX_PCR_MODE_NONE;
    rPid.ePidType = DMX_PID_TYPE_PSI;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = DMX_DEFAULT_PSI_FIFO_SIZE;
    rPid.u2Pid = u2Pid;
    rPid.pfnNotify = NULL;
    rPid.pvNotifyTag = NULL;

    if (!DMX_SetPid(u1PidIndex, DMX_PID_FLAG_ALL, &rPid))
    {
        return -1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _PrintSetFilterUsage(const CHAR* szCmd)
{
    Printf("%s index bank pid_index offset mode crc byte0 ... byte7"
           " [mask_word0 mask_word1]\n", szCmd);
    Printf("    index: 0 - %u\n", DMX_NUM_FILTER_INDEX - 1);
    Printf("    bank: 0 - %u\n", DMX_NUM_FILTER_BANK - 1);
    Printf("    pid_index: 0 - %u\n", DMX_NUM_PID_INDEX - 1);
    Printf("    offset: %u - %u\n", DMX_MIN_FILTER_OFFSET,
           DMX_MAX_FILTER_OFFSET);
    Printf("    mode: short, long, pos_neg, 14_2\n");
    Printf("    crc: on, off\n");
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetFilter(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_FILTER_T rFilter;
    UINT32 i;
    UINT8 u1FilterIndex, u1Bank, au1Data[8];

    ASSERT(aszArgv != NULL);

    if (i4Argc < 15)
    {
        _PrintSetFilterUsage(aszArgv[0]);
        return -1;
    }

    x_memset(&rFilter, 0, sizeof(rFilter));
    rFilter.fgEnable = TRUE;

    u1FilterIndex = (UINT8)StrToInt(aszArgv[1]);
    u1Bank = (UINT8)StrToInt(aszArgv[2]);
    rFilter.u1Pidx = (UINT8)StrToInt(aszArgv[3]);
    rFilter.u1Offset = (UINT8)StrToInt(aszArgv[4]);
    if ((u1FilterIndex >= DMX_NUM_FILTER_INDEX) ||
            (u1Bank >= DMX_NUM_FILTER_BANK) ||
            (rFilter.u1Pidx >= DMX_NUM_PID_INDEX) ||
//      (rFilter.u1Offset < DMX_MIN_FILTER_OFFSET) ||
            (rFilter.u1Offset > DMX_MAX_FILTER_OFFSET))
    {
        _PrintSetFilterUsage(aszArgv[0]);
        return -1;
    }

    if (x_strcmp(aszArgv[5], "short") == 0)
    {
        rFilter.eMode = DMX_FILTER_MODE_SHORT;
    }
    else if (x_strcmp(aszArgv[5], "long") == 0)
    {
        rFilter.eMode = DMX_FILTER_MODE_LONG;
    }
    else if (x_strcmp(aszArgv[5], "pos_neg") == 0)
    {
        rFilter.eMode = DMX_FILTER_MODE_POS_NEG;
    }
    else if (x_strcmp(aszArgv[5], "14_2") == 0)
    {
        rFilter.eMode = DMX_FILTER_MODE_14_2;
    }
    else
    {
        _PrintSetFilterUsage(aszArgv[0]);
        return -1;
    }

    if (x_strcmp(aszArgv[6], "on") == 0)
    {
        rFilter.fgCheckCrc = TRUE;
    }
    else if (x_strcmp(aszArgv[6], "off") == 0)
    {
        rFilter.fgCheckCrc = FALSE;
    }
    else
    {
        _PrintSetFilterUsage(aszArgv[0]);
        return -1;
    }

    for (i = 7; i < 15; i++)
    {
        au1Data[i - 7] = (UINT8)StrToInt(aszArgv[i]);
    }

    rFilter.au4Data[0] = (au1Data[0] << 24 ) | (au1Data[1] << 16) |
                         (au1Data[2] << 8) | au1Data[3];
    rFilter.au4Data[1] = (au1Data[4] << 24 ) | (au1Data[5] << 16) |
                         (au1Data[6] << 8) | au1Data[7];

    rFilter.au4Mask[0] = 0xffffffff;
    rFilter.au4Mask[1] = 0xffffffff;
    if (i4Argc > 15)
    {
        rFilter.au4Mask[0] = (UINT32)StrToInt(aszArgv[15]);
    }
    if (i4Argc > 16)
    {
        rFilter.au4Mask[1] = (UINT32)StrToInt(aszArgv[16]);
    }

    if (!DMX_SetFilter(u1FilterIndex, u1Bank, DMX_FILTER_FLAG_ALL, &rFilter))
    {
        return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_GetFilter(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_FILTER_T rFilter;
    UINT32 i, j;
    UINT8 u1Pidx, u1Bank;

    ASSERT(aszArgv != NULL);

    if (i4Argc > 3)
    {
        Printf("Usage: %s [all|(pid_index [bank])]\n", aszArgv[0]);
        return -1;
    }

    if ((i4Argc == 1) || (x_strcmp(aszArgv[1], "all") == 0))
    {
        for (i = 0; i < DMX_NUM_FILTER_INDEX; i++)
        {
            for (j = 0; j < DMX_NUM_FILTER_BANK; j++)
            {
                if (!DMX_GetFilter(i, j, DMX_FILTER_FLAG_ALL, &rFilter))
                {
                    return -1;
                }

                if (rFilter.fgEnable)
                {
                    _ShowFilterInfo(i, j, &rFilter);
                }
            }
        }
    }
    else
    {
        u1Pidx = (UINT8)StrToInt(aszArgv[1]);
        u1Bank = 0;
        if (i4Argc > 2)
        {
            u1Bank = (UINT8)StrToInt(aszArgv[2]);
        }

        if (!DMX_GetFilter(u1Pidx, u1Bank, DMX_FILTER_FLAG_ALL, &rFilter))
        {
            return -1;
        }

        _ShowFilterInfo(u1Pidx, u1Bank, &rFilter);
    }

    return 0;
}


#if defined(CC_MT5399) || defined(CC_MT5890)

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_GetGenericFilter(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_FILTER_GENERIC_T rFilter;
    UINT32 i;
    UINT8 u1FilterIndex;
    UINT8 u1Tsidx;

    ASSERT(aszArgv != NULL);

    if (i4Argc < 2)
    {
        Printf("Usage: %s Tsindex [all|pid_index]\n", aszArgv[0]);
        return -1;
    }

    u1Tsidx= (UINT8)StrToInt(aszArgv[1]);

    if ((i4Argc == 2) || (x_strcmp(aszArgv[2], "all") == 0))
    {
        for (i = 0; i < DMX_NUM_FILTER_INDEX; i++)
        {
            rFilter.u1Tsidx = u1Tsidx;
            if (!DMX_GetGenFilter(i, DMX_FILTER_FLAG_ALL, &rFilter))
            {
                return -1;
            }
            if (rFilter.fgEnable)
            {
                _ShowGenericFilterInfo(i, &rFilter);
            }
        }
    }
    else
    {
        u1FilterIndex = (UINT8)StrToInt(aszArgv[2]);
        rFilter.u1Tsidx = u1Tsidx;
        if (!DMX_GetGenFilter(u1FilterIndex, DMX_FILTER_FLAG_ALL, &rFilter))
        {
            return -1;
        }
        _ShowGenericFilterInfo(u1FilterIndex, &rFilter);
    }

    return 0;
}

#else
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_GetGenericFilter(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_FILTER_GENERIC_T rFilter;
    UINT32 i;
    UINT8 u1FilterIndex;

    ASSERT(aszArgv != NULL);

    if (i4Argc > 2)
    {
        Printf("Usage: %s [all|pid_index]\n", aszArgv[0]);
        return -1;
    }

    if ((i4Argc == 1) || (x_strcmp(aszArgv[1], "all") == 0))
    {
        for (i = 0; i < DMX_NUM_FILTER_INDEX; i++)
        {
            if (!DMX_GetGenFilter(i, DMX_FILTER_FLAG_ALL, &rFilter))
            {
                return -1;
            }
            if (rFilter.fgEnable)
            {
                _ShowGenericFilterInfo(i, &rFilter);
            }
        }
    }
    else
    {
        u1FilterIndex = (UINT8)StrToInt(aszArgv[1]);
        if (!DMX_GetGenFilter(u1FilterIndex, DMX_FILTER_FLAG_ALL, &rFilter))
        {
            return -1;
        }
        _ShowGenericFilterInfo(u1FilterIndex, &rFilter);
    }

    return 0;
}
#endif

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_GetCounters(INT32 i4Argc, const CHAR** aszArgv)
{
    ASSERT(aszArgv != NULL);

    if (i4Argc == 1)
    {
        // Show all counters of active PIDs
        DMX_PID_T rPid;
        UINT32 i;

        for (i = 0; i < DMX_NUM_PID_INDEX; i++)
        {
            if (DMX_CLIGetPid(i, DMX_PID_FLAG_VALID, &rPid))
            {
                if (rPid.fgEnable)
                {
                    _ShowPidCounters(i);
                }
            }
        }
    }
    else
    {
        UINT8 u1Pidx;

        u1Pidx = (UINT8)StrToInt(aszArgv[1]);
        _ShowPidCounters(u1Pidx);
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_ResetCounters(INT32 i4Argc, const CHAR** aszArgv)
{
    ASSERT(aszArgv != NULL);

    if (i4Argc == 1)
    {
        // Reset all counters
        UINT8 i;

        for (i = 0; i < DMX_NUM_PID_INDEX; i++)
        {
            if (!DMX_ResetPidCounters(i))
            {
                Printf("Fail to reset counters of PID index %u\n", i);
                return -1;
            }
        }
    }
    else
    {
        UINT8 u1Pidx;

        u1Pidx = (UINT8)StrToInt(aszArgv[1]);
        if (!DMX_ResetPidCounters(u1Pidx))
        {
            Printf("Fail to reset counters of PID index %u\n", u1Pidx);
            return -1;
        }
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_Query(INT32 i4Argc, const CHAR** aszArgv)
{
    ASSERT(aszArgv != NULL);

    if (i4Argc == 1)
    {
        // Show all counters of active PIDs
        DMX_PID_T rPid;
        UINT32 i;

        for (i = 0; i < DMX_NUM_PID_INDEX; i++)
        {
            if (DMX_CLIGetPid(i, DMX_PID_FLAG_VALID, &rPid))
            {
                if (rPid.fgEnable)
                {
                    _QueryPid(i);
                }
            }
        }
    }
    else
    {
        UINT8 u1Pidx;

        u1Pidx = (UINT8)StrToInt(aszArgv[1]);
        _QueryPid(u1Pidx);
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_MULPidQuery(INT32 i4Argc, const CHAR** aszArgv)
{
    ASSERT(aszArgv != NULL);

    if (i4Argc == 1)
    {
        // Show all counters of active PIDs
        UINT32 i;

        for (i = 0; i < DMX_NUM_PID_INDEX; i++)
        {
            _QueryMulPid(i);
        }
    }
    else
    {
        UINT8 u1Pidx;

        u1Pidx = (UINT8)StrToInt(aszArgv[1]);
        _QueryMulPid(u1Pidx);
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_MULInstQuery(INT32 i4Argc, const CHAR** aszArgv)
{
    ASSERT(aszArgv != NULL);

    if (i4Argc == 1)
    {
        // Show all counters of active Insts
        UINT32 i;

        for (i = 0; i < DMX_MULTIPLE_INSTANCE_MAXNUM; i++)
        {
            _QueryMulInst(i);
        }
    }
    else
    {
        UINT8 u1Pidx;

        u1Pidx = (UINT8)StrToInt(aszArgv[1]);
        _QueryMulInst(u1Pidx);
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_RcvPktCount(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MsTime, u4RcvCount, u4DropCount, u4DBMStatus, u4FramerStatus;
    UINT8 u1Framer;

    u1Framer = DMX_GetFramerIndex();

    u4MsTime = 100;

    if (i4Argc < 2)
    {
        Printf("%s ms_sec framer\n", aszArgv[0]);
        Printf("--------------------------------------------------\n");
    }

    if (i4Argc >= 2)
    {
        u4MsTime = StrToInt(aszArgv[1]);
    }

    if (i4Argc >= 3)
    {
        u1Framer = StrToInt(aszArgv[2]);
    }

    if (u1Framer > 3)
    {
        Printf("Framer exceed\n");
        return -1;
    }

    DMX_GetPacketStatus(u1Framer, u4MsTime, &u4RcvCount, &u4DropCount);
    DMX_GetDBMFramerStatus(u1Framer, &u4DBMStatus, &u4FramerStatus);

    Printf("Framer: %u, Period: %u ms, receive %u pkt, drop %u pkt\n",
           u1Framer, u4MsTime, u4RcvCount, u4DropCount);
    Printf("DBM Status: 0x%x, Framer Status : 0x%x!\n",
           u4DBMStatus, u4FramerStatus);
    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_Version(INT32 i4Argc, const CHAR** aszArgv)
{
    UNUSED(i4Argc);
    UNUSED(aszArgv);

    DMX_Version();
#ifdef CC_FBM_SUPPORT_DMXPID
    DMX_PSIBufferStatus();
#endif

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_GetEfuseStatus(INT32 i4Argc, const CHAR** aszArgv)
{
    UNUSED(i4Argc);
    UNUSED(aszArgv);

    if (DMX_GetEfuseStatus())
    {
        Printf("IC Status: Efuse IC!\n");
    }
    else
    {
        Printf("IC Status: Not Efuse IC!\n");
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_Power(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgEnable;

    if (i4Argc != 2)
    {
        Printf("%s 0 | 1\n", aszArgv[0]);
        Printf("    0: off\n");
        Printf("    1: on\n");
        return 0;
    }

    fgEnable = (StrToInt(aszArgv[1]) == 0) ? FALSE : TRUE;

    DMX_EnablePower(fgEnable);

    Printf("OK\n");

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  The handler routine for the capture function in the one-shot mode.
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static BOOL _CaptureHandlerOneshotMode(DMX_CAPTURE_NOTIFY_CODE_T eCode,
                                       const DMX_CAPTURE_INFO_T* prInfo, void* pvTag)
{
    DMX_CAPTURE_T rCapture;

    x_memset((void*)&rCapture, 0, sizeof(DMX_CAPTURE_T));
    DMX_SetCapture(FALSE, &rCapture);

    _fgCaptureEnable = FALSE;

    LOG(0, "Capture finish\n");

    return TRUE;
}


//-----------------------------------------------------------------------------
/** Function
 *  The handler routine for the capture function in the continuous mode.
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static BOOL _CaptureHandlerContinuousMode(DMX_CAPTURE_NOTIFY_CODE_T eCode,
        const DMX_CAPTURE_INFO_T* prInfo, void* pvTag)
{
    DMX_UpdateCaptureReadPointer(prInfo->u4Wp);
    LOG(5, "CapBuf start:0x%08X, end:0x%08X, RP:0x%08X, WP:0x%08X\n",
        prInfo->u4BufStart, prInfo->u4BufEnd, prInfo->u4Rp, prInfo->u4Wp);

    return TRUE;
}


//-----------------------------------------------------------------------------
static void _PrintCaptureUsage(const CHAR* str)
{
    Printf("Usage: %s [on|off] [bypass_dev] [oneshot|loop]\n", str);
    Printf("   bypass_dev: [none | framer0 | framer1 | pvr | ddi]\n");
    Printf("        0: none\n");
    Printf("        1: framer0\n");
    Printf("        2: framer1\n");
    Printf("        3: pvr\n");
    Printf("        4: ddi\n");
    Printf("   default mode: oneshot\n");
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_Capture(INT32 i4Argc, const CHAR ** aszArgv)
{
    DMX_CAPTURE_T rCapture;
    UINT32 u4Dev, u4BypassDev;
    BOOL fgEnable;

    ASSERT(aszArgv != NULL);

    if ((i4Argc < 3) || (i4Argc > 4))
    {
        _PrintCaptureUsage(aszArgv[0]);
        return -1;
    }

    u4Dev = StrToInt(aszArgv[2]);
    switch (u4Dev)
    {
    case 0:
        u4BypassDev = (UINT32)DMX_CAPTURE_BYPASS_NONE;
        break;
    case 1:
        u4BypassDev = (UINT32)DMX_CAPTURE_BYPASS_FRAMER0;
        break;
    case 2:
        u4BypassDev = (UINT32)DMX_CAPTURE_BYPASS_FRAMER1;
        break;
    case 3:
        u4BypassDev = (UINT32)DMX_CAPTURE_BYPASS_PVR;
        break;
    case 4:
        u4BypassDev = (UINT32)DMX_CAPTURE_BYPASS_DDI;
        break;
    default:
        _PrintCaptureUsage(aszArgv[0]);
        return -1;
    }

    if (x_strcmp(aszArgv[1], "on") == 0)
    {
        fgEnable = TRUE;
    }
    else if (x_strcmp(aszArgv[1], "off") == 0)
    {
        fgEnable = FALSE;
    }
    else
    {
        _PrintCaptureUsage(aszArgv[0]);
        return -1;
    }

    if (fgEnable)
    {
        if (_fgCaptureEnable)
        {
            Printf("Already enable\n");
            return -1;
        }

        if ((_u4CaptureBufAddr == 0) || (_u4CaptureBufSize == 0))
        {
            Printf("Please call capturebuf CLI to allocate buffer first\n");
            return -1;
        }

        rCapture.u4BytePassDev = u4BypassDev;
        rCapture.u4BufStart = _u4CaptureBufAddr;
        rCapture.u4BufSize = _u4CaptureBufSize;
        rCapture.u4Threshold = _u4CaptureBufSize - 1024;
        rCapture.pvCaptureTag = NULL;

        // Determine to use the One-shot mode or the Continuous mode?
        // Default: one-shot mode
        if (i4Argc == 4)
        {
            if (x_strcmp(aszArgv[3], "oneshot") == 0)
            {
                rCapture.pfnCaptureHandler = _CaptureHandlerOneshotMode;
            }
            else if (x_strcmp(aszArgv[3], "loop") == 0)
            {
                rCapture.pfnCaptureHandler = _CaptureHandlerContinuousMode;
            }
            else
            {
                _PrintCaptureUsage(aszArgv[0]);
                return -1;
            }
        }
        else
        {
            rCapture.pfnCaptureHandler = _CaptureHandlerOneshotMode;
        }

        if (!DMX_SetCapture(TRUE, &rCapture))
        {
            Printf("Fail to enable capture\n");
            return -1;
        }

        _fgCaptureEnable = TRUE;

        Printf("Capture start...\n");
    }
    else
    {
        x_memset((void*)&rCapture, 0, sizeof(DMX_CAPTURE_T));
        if (!DMX_SetCapture(FALSE, &rCapture))
        {
            Printf("Fail to disable capture\n");
            return -1;
        }

        _fgCaptureEnable = FALSE;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_CaptureAllocBuf
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_CaptureAllocBuf(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Size;
    FBM_POOL_T* prFbmPool;
    static BOOL _fgUsePVRBuf = FALSE;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 2)
    {
        Printf("%s [size]\n", aszArgv[0]);
        return 0;
    }

    u4Size = StrToInt(aszArgv[1]);

    if ((!_fgUsePVRBuf) && (_u4CaptureBufAddr != 0))
    {
        if (!BSP_FreeAlignedDmaMemory(_u4CaptureBufAddr))
        {
            return -1;
        }
    }

    if (0 == u4Size)
    {
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_PVR);
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != 0);

        _u4CaptureBufAddr = prFbmPool->u4Addr;
        _u4CaptureBufSize = prFbmPool->u4Size;
        _fgUsePVRBuf = TRUE;
    }
    else
    {
        _u4CaptureBufAddr = 0;
        _u4CaptureBufSize = 0;

        // Do not use x_mem_alloc() because of the cache consistency problem.
        _u4CaptureBufAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4Size,
                            DMX_CA_BUFFER_ALIGNMENT);
        if (_u4CaptureBufAddr == 0)
        {
            Printf("Allocate buffer fail!\n");
            return -1;
        }

        _u4CaptureBufSize = u4Size;
        _fgUsePVRBuf = FALSE;
    }

    Printf("Buffer addr: 0x%x, size: 0x%x\n", VIRTUAL(_u4CaptureBufAddr), _u4CaptureBufSize);

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_CaptureInfo
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_CaptureInfo(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_CAPTURE_INFO_T rInfo;

    if (!DMX_GetCaptureInfo(&rInfo))
    {
        return -1;
    }

//    Printf("Buffer: 0x%x ~ 0x%x, size: 0x%x\n", rInfo.u4BufStart, rInfo.u4BufEnd,
//                    rInfo.u4BufEnd - rInfo.u4BufStart);
    Printf("Buffer: 0x%x ~ 0x%x\n", VIRTUAL(rInfo.u4Rp), VIRTUAL(rInfo.u4Wp));
    Printf("Datasize: 0x%x\n", rInfo.u4DataSize);

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_CaptureInfo
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_ChangeChannelCaptureInit(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Size;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 2)
    {
        Printf("%s [size]\n", aszArgv[0]);
        return 0;
    }

    u4Size = StrToInt(aszArgv[1]);

    DMX_Capture_ChannelChangeInit(u4Size);

    return 0;
}


static INT32 _DMXCLI_ChangeChannelCapture(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_InformChannelChange();

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_SetErrLevel
 */
//-----------------------------------------------------------------------------
EXTERN void _DMX_PanicDisplay(void);
static INT32 _DMXCLI_SetErrLevel(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4ErrLevel;

    _DMX_PanicDisplay();

    if (i4Argc != 2)
    {
        Printf("%s 0|1|2\n", aszArgv[0]);
        Printf("   0:assert, 1:reset, 2:bypass\n");
        return 0;
    }

    u4ErrLevel = StrToInt(aszArgv[1]);
    if (u4ErrLevel >= 3)
    {
        Printf("%s on|off\n", aszArgv[0]);
        Printf("   on|off: reset on or off\n");
        return -1;
    }

    DMX_SetErrLevel((DMX_ERR_LEVEL_T)u4ErrLevel);

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_SetScrambleScheme
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetScrambleScheme(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1Mode = 0x0;
    UINT8 u1Flag = 0x0;
    DMX_SCRAMBLE_TYPE_T eType;

    if (i4Argc < 2)
    {
        Printf("%s 0|1|2 [flag]\n", aszArgv[0]);
        Printf("  0: TSFLAG_ONLY.\n");
        Printf("  1: PESSTART_ONLY.\n");
        Printf("  2: BOTH_TSFLAG_PESSTART. must add the flag.\n");
        Printf("  	 flag = 1 : pes=0, flag=1 as scramble.\n");
        Printf("  	 flag = 2 : pes=1, flag=0 as scramble.\n");
        Printf("  	 flag = 3 : pes=1, flag=1 as scramble.\n");
        Printf("********************************************.\n");
        if (!DMX_GetScrambleSchemeEx(&eType, &u1Flag))
        {
            return -1;
        }

        switch (eType)
        {
        case DMX_SCRAMBLE_TSFLAG_ONLY:
            Printf("ScrambleScheme: TSFLAG_ONLY.\n");
            break;

        case DMX_SCRAMBLE_PESSTART_ONLY:
            Printf("ScrambleScheme: PESSTART_ONLY.\n");
            break;

        case DMX_SCRAMBLE_BOTH_TSFLAG_PESSTART:
            Printf("ScrambleScheme: BOTH_TSFLAG_PESSTART, FLAG: %d.\n", u1Flag);
            break;

        default:
            Printf("Error happen!.\n");
            break;
        }
        return 0;
    }

    u1Mode = (UINT8)StrToInt(aszArgv[1]);

    if (u1Mode == 2)
    {
        if (i4Argc < 3)
        {
            Printf("%s 0|1|2 [flag]\n", aszArgv[0]);
            Printf("You must type the [flag]\n");
            return -1;
        }
    }

    u1Flag = (UINT8)StrToInt(aszArgv[2]);

    if (u1Mode > 2)
    {
        Printf("%s 0|1|2 [flag]\n", aszArgv[0]);
        Printf("Please type the right mode!\n");
        return -1;
    }

    switch (u1Mode)
    {
    case 0:
        eType = DMX_SCRAMBLE_TSFLAG_ONLY;
        break;

    case 1:
        eType = DMX_SCRAMBLE_PESSTART_ONLY;
        break;

    case 2:
        eType = DMX_SCRAMBLE_BOTH_TSFLAG_PESSTART;
        break;

    default:
        eType = DMX_SCRAMBLE_PESSTART_ONLY;
        break;
    }

    DMX_SetScrambleSchemeEx(eType, u1Flag);

    Printf("Set new scramble scheme done!\n");

    return 0;
}

//-----------------------------------------------------------------------------
/** _DMXCLI_GetScrambleState
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_GetScrambleState(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Pidx;
    DMX_SCRAMBLE_STATE_T eState;

    ASSERT(aszArgv != NULL);

    if (i4Argc < 2)
    {
        return -1;
    }

    u4Pidx = StrToInt(aszArgv[1]);
    if (u4Pidx >= DMX_NUM_PID_INDEX)
    {
        return -1;
    }

    eState = DMX_GetScrambleState(u4Pidx);

    switch (eState)
    {
    case DMX_SCRAMBLE_STATE_CLEAR:
        Printf("Pidx: %d, Scramble state: %s.\n", u4Pidx, "CLEAR");
        break;

    case DMX_SCRAMBLE_STATE_SCRAMBLED:
        Printf("Pidx: %d, Scramble state: %s.\n", u4Pidx, "SCRAMBLED");
        break;

    default:
        Printf("Pidx: %d, Scramble state: %s.\n", u4Pidx, "UNKNOWN");
        break;
    }

    return 0;
}


#ifdef TIME_SHIFT_SUPPORT
//-----------------------------------------------------------------------------
/** _DMXCLI_TimeShiftQuery
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_TimeShiftQuery(INT32 i4Argc, const CHAR** aszArgv)
{
		FVR_QUERY_INFO_T rInfo;
		FVR_PID_T rFVRPid;
		UINT8 i;
		DMX_PVRPLAY_COUNTER_T rCounter;
		BOOL fgIsRecording = FALSE;
	
		//------------------------------------------------------------
		// FVR buffer
		//------------------------------------------------------------
	
		for(i = 0; i < FVR_GBL_COUNT; i++)
		{
			if (FVR_IsRecording(i))
			{
				Printf("FVR %d is recording\n", i);
				fgIsRecording = TRUE;
			}
			else
			{
				continue;
			}
	
			FVR_QueryStatus(i, &rInfo);
			Printf("Receive pkt count: %d\n", rInfo.u4PktCount);
			Printf("Buf: 0x%08x ~ 0x%08x, WP: 0x%08x, RP: 0x%08x\n",
				   rInfo.u4BufStart, rInfo.u4BufEnd, rInfo.u4Wp, rInfo.u4Rp);
			Printf("Buf peak: 0x%x\n", rInfo.u4PktCount);
			Printf("Overflow count: %d\n", rInfo.u4OverflowCount);
		}
	
		if(!fgIsRecording)
		{
			Printf("FVR stop record\n");
		}
	
		//------------------------------------------------------------
		// Reocrd PID
		//------------------------------------------------------------
		for (i=0; i<FVR_NUM_PID_INDEX; i++)
		{
			if (!FVR_GetPid(i, FVR_PID_FLAG_ENABLE | FVR_PID_FLAG_TSINDEX | FVR_PID_FLAG_PID | FVR_PID_FLAG_PICSEARCH |
							FVR_PID_FLAG_PKTSWAP | FVR_PID_FLAG_SCRAMBLE_ENABLE, &rFVRPid))
			{
				return -1;
			}
	
			if (rFVRPid.fgEnable)
			{
				Printf("Pidx: %02d, TS_idx: %02d, Pid: 0x%x enable, pic_serach:%d, swap:%d, encry:%d\n",
					   i, rFVRPid.u1TsIndex, rFVRPid.u2Pid, rFVRPid.fgPicSearch, rFVRPid.fgPktSwap,
					   rFVRPid.fgScramble);
			}
		}
	
		Printf("\n");
	
		//------------------------------------------------------------
		// Play info
		//------------------------------------------------------------
		for(i = 0; i < DMX_MAX_TSOUT; i++)
		{
			DMX_PVRPlay_GetCounterEx(i, &rCounter);
			Printf("Play %d:\n", i);
			Printf("Use timestamp: %d\n", (INT32)rCounter.fgUseTimestamp);
			Printf("x1:Move, count:%d, check:%d, size:0x%x\n",
				   rCounter.u4MoveCount, rCounter.u4MoveCheck, rCounter.u4MoveSize);
			Printf("not1:Move, count:%d, check:%d, size:0x%x\n",
				   rCounter.u4SingleCount, rCounter.u4SingleCheck, rCounter.u4SingleSize);
			Printf("Datasize: 0x%x\n", rCounter.u4DataSize);
		}
	
		return 0;
}



//-----------------------------------------------------------------------------
/** _DMXCLI_TimeShiftResetCounter
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_TimeShiftResetCounter(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PVRPlay_ResetCounter();

    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}
#endif  // TIME_SHIFT_SUPPORT


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetVideoType(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_VIDEO_TYPE_T eVideoType;

    if (i4Argc != 2)
    {
        Printf("%s [mpeg|h264|avs|wmv...]\n", aszArgv[0]);
        Printf("      mpeg:0, h264:1, avs:2, wvm:3...\n");
        return 0;
    }

    eVideoType = (DMX_VIDEO_TYPE_T)StrToInt(aszArgv[1]);

    if (!DMX_SetVideoType(eVideoType))
    {
        return -1;
    }

    return 0;
}

#ifdef CC_DMX_ERR_RECOVERY_TEST
//-----------------------------------------------------------------------------
/** _DMXCLI_PanicTest
 */
//-----------------------------------------------------------------------------
BOOL DMX_Test_Enable = FALSE;
static INT32 _DMXCLI_PanicTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Loop, u4Period, i;

    if (i4Argc != 3)
    {
        Printf("%s loop_count period_ms\n", aszArgv[0]);
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u4Period = StrToInt(aszArgv[2]);

    for (i=0; i<u4Loop; i++)
    {
        Printf("------ %d ------\n", i);
        DMX_Test_Enable = TRUE;
        x_thread_delay(u4Period);
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_MonPanicTest
 */
//-----------------------------------------------------------------------------
EXTERN void _DMX_PanicMicroCode(void);
static INT32 _DMXCLI_MonPanicTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Loop, u4Period, i;

    if (i4Argc != 3)
    {
        Printf("%s loop_count period_ms\n", aszArgv[0]);
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u4Period = StrToInt(aszArgv[2]);

    for (i=0; i<u4Loop; i++)
    {
        Printf("------ %d ------\n", i);
        _DMX_PanicMicroCode();
        x_thread_delay(u4Period);
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** DMXCLI_FVROverflow
*/
//-----------------------------------------------------------------------------
extern void _FVR_uPErrorInt(UINT8 u1Idx);
static INT32 DMXCLI_FVROverflow(INT32 i4Argc, const CHAR** aszArgv)
{
    _FVR_uPErrorInt(0);

    return 0;
}
#endif  // CC_DMX_ERR_RECOVERY_TEST

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_CheckAW(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4RegVal;
    BOOL fgDriverSupport = FALSE, fguCodeSupport = FALSE;
    //-------------------------------------------------------------------------
    // --- Beginning of "check for AW Eraser" ---
    const UINT32 u4Timeout = 500;
    INT32 i4Result;
    // --- End of "check for AW Eraser" ---
    //-------------------------------------------------------------------------

    if (i4Argc != 1)
    {
        Printf("Usage: %s\n", aszArgv[0]);
        return -1;
    }

    u4RegVal = BIM_READ32(0x664);
    if ((u4RegVal & 0x4) == 0x4)
    {
        // Demod does not clear VCT.
        Printf("J-Chip feature: OFF\n");
    }
    else
    {
        // Demod clears VCT.
        Printf("J-Chip feature: ON\n");
    }

    DMX_CheckAwSupport(&fgDriverSupport, &fguCodeSupport);

    Printf("AW support in driver: %s\n", fgDriverSupport ? "YES" : "NO");
    Printf("AW support in micro-code: %s\n", fguCodeSupport ? "YES" : "NO");

    //-------------------------------------------------------------------------
    // --- Beginning of "check for AW Eraser" ---
    DMX_AwEraserCheck_Init();
    DMX_AwEraserCheck_Set(TRUE);
    i4Result = DMX_AwEraserCheck_Get(u4Timeout,_u1TsIndex);
    DMX_AwEraserCheck_Free();
    Printf("AW eraser check result: ");
    switch (i4Result)
    {
    case DMX_AW_ERR_OK:
        Printf("OK\n");
        break;
    case DMX_AW_ERR_ADDRESS_RANGE:
        Printf("out of address range\n");
        break;
    case DMX_AW_ERR_SECTION_LEN:
        Printf("incorrect section length\n");
        break;
    case DMX_AW_ERR_NO_MEM:
        Printf("out of memory\n");
        break;
    case DMX_AW_ERR_COPY_BUFFER:
        Printf("copy error\n");
        break;
    case DMX_AW_ERR_INSUFFICIENT_LEN:
        Printf("insufficient length\n");
        break;
    case DMX_AW_ERR_NOT_ZEROED_OUT:
        Printf("VCT/SLD not cleared\n");
        break;
    case DMX_AW_ERR_TIMEOUT:
        Printf("timeout\n");
        break;
    default:
        Printf("unknown error\n");
        break;
    }

    if (i4Result)
    {
        return i4Result;
    }
    // --- End of "check for AW Eraser" ---
    //-------------------------------------------------------------------------

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Error code
 */
//-----------------------------------------------------------------------------
static BOOL _ParseSection(const UINT8* au1Buf, UINT32 u4Size,
                          SECTION_INFO_T* prSecInfo)
{
    if (u4Size < 3)
    {
        return FALSE;
    }

    if ((au1Buf == NULL) || (prSecInfo == NULL))
    {
        return FALSE;
    }

    x_memset(prSecInfo, 0, sizeof(SECTION_INFO_T));

    prSecInfo->u1TableId = au1Buf[0];
    prSecInfo->fgSectionSyntaxIndicator = ((au1Buf[1] & 0x80) != 0);
    prSecInfo->fgPrivateIndicator = ((au1Buf[1] & 0x40) != 0);
    prSecInfo->u2SectionLength = ((au1Buf[1] & 0xf) << 8) | au1Buf[2];
    if (prSecInfo->fgSectionSyntaxIndicator)
    {
        if (u4Size < 8)
        {
            return FALSE;
        }

        prSecInfo->u2TableIdExtension = (au1Buf[3] << 8) | au1Buf[4];
        prSecInfo->u1VersionNumber = (au1Buf[5] & 0x3e) >> 1;
        prSecInfo->fgCurrentNextIndicator = ((au1Buf[5] & 1) != 0);
        prSecInfo->u1SectionNumber = au1Buf[6];
        prSecInfo->u1LastSectionNumber = au1Buf[7];
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Error code
 */
//-----------------------------------------------------------------------------
static BOOL _SectionHandler(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
                            UINT32 u4Data, const void* pvNotifyTag)
{
    SECTION_INFO_T rSecInfo;
    UINT32 u4Size;
    DMX_NOTIFY_INFO_PSI_T* prInfo;
    UINT8 au1Buf[16];

    ASSERT(eCode == DMX_NOTIFY_CODE_PSI);
    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    prInfo = (DMX_NOTIFY_INFO_PSI_T*)u4Data;
    ASSERT(prInfo != NULL);

    u4Size = MIN(prInfo->u4SecLen, 16);
    VERIFY(DMX_PSI_GetBuffer(u1Pidx, prInfo->u1SerialNumber, prInfo->u4SecAddr,
                             0, u4Size, au1Buf));
    VERIFY(_ParseSection(au1Buf, u4Size, &rSecInfo));

    LOG(0, "  Section - pidx: %u, section: 0x%08x, size: %u\n",
        u1Pidx, prInfo->u4SecAddr, prInfo->u4SecLen);

    LOG(0, "  Data: %02x  %02x  %02x  %02x  %02x  %02x  %02x %02x\n",
        au1Buf[0], au1Buf[1], au1Buf[2], au1Buf[3], au1Buf[4], au1Buf[5],
        au1Buf[6], au1Buf[7]);

    LOG(0, "  table_id: 0x%02x, section_syntax_indicator: %u, section_length: %u\n",
        rSecInfo.u1TableId, rSecInfo.fgSectionSyntaxIndicator,
        rSecInfo.u2SectionLength);

    LOG(0, "  table_id_extension: 0x%04x, version_number: %u, section_number: %u\n\n",
        rSecInfo.u2TableIdExtension, rSecInfo.u1VersionNumber,
        rSecInfo.u1SectionNumber);

    if (!DMX_PSI_UnlockBuffer(u1Pidx, prInfo->u1SerialNumber, prInfo->u4SecLen,
                              prInfo->u4SecAddr))
    {
        LOG(0, "Fail to unlock PSI buffer!\n");
        return FALSE;
    }

    UNUSED(pvNotifyTag);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_FreePATSection(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PID_T rPid;
    DMX_FILTER_GENERIC_T rFilter;

    //free section filter.
    x_memset(&rFilter, 0, sizeof(rFilter));
    rFilter.fgEnable = FALSE;
    if (!DMX_SetGenFilter(_u1FilterIndex, DMX_FILTER_FLAG_VALID, &rFilter))
    {
        return -1;
    }

    //free pid
    rPid.fgEnable = FALSE;

    if (!DMX_SetPid(_u1PidIndex, DMX_PID_FLAG_VALID, &rPid))
    {
        Printf("Disable pid fail.\n");
        return -1;
    }

    if (!DMX_FreePid(_u1PidIndex))
    {
        Printf("Free pid fail.\n");
        return -1;
    }

    Printf("Free section filter and pid filter ok.\n");

    UNUSED(i4Argc);
    UNUSED(aszArgv);
    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_TestPATSection(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PID_T rPid;
    DMX_FILTER_GENERIC_T rFilter;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 1)
    {
        Printf("Usage: %s.\n", aszArgv[0]);
        return -1;
    }
    rPid.u1TsIndex = _u1TsIndex;
    rPid.u1DeviceId = 0;
    rPid.u1KeyIndex = 0;
    rPid.fgEnable = TRUE;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.fgAllocateBuffer = TRUE;
    rPid.ePcrMode = DMX_PCR_MODE_NONE;
    rPid.ePidType = DMX_PID_TYPE_PSI;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = DMX_DEFAULT_PSI_FIFO_SIZE;
    rPid.u2Pid = 0x0;
    rPid.pfnNotify = _SectionHandler;
    rPid.pvNotifyTag = NULL;

    if (!DMX_SetPid(_u1PidIndex, DMX_PID_FLAG_ALL, &rPid))
    {
        return -1;
    }

    //Set PAT section filter

    x_memset(&rFilter, 0, sizeof(rFilter));
    rFilter.fgEnable = TRUE;

    rFilter.u1Pidx = _u1PidIndex;
    rFilter.u1Offset = 3;
    rFilter.fgCheckCrc = TRUE;
    rFilter.au1Data[0] = 0x0;
    rFilter.au1Mask[0] = 0xFF;
    rFilter.eMode = DMX_GENFILTER_8_BYTE;

    if (!DMX_SetGenFilter(_u1FilterIndex, DMX_FILTER_FLAG_ALL, &rFilter))
    {
        return -1;
    }

    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetPsiPidCallback(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PID_T rPid;
    UINT16 u2Pid;
    UINT8 u1PidIndex;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 3)
    {
        Printf("Usage: %s pid_index pid\n", aszArgv[0]);
        return -1;
    }

    u1PidIndex = (UINT8)StrToInt(aszArgv[1]);
    u2Pid = (UINT16)StrToInt(aszArgv[2]);

    rPid.u1TsIndex = _u1TsIndex;
    rPid.u1DeviceId = 0;
    rPid.u1KeyIndex = 0;
    rPid.fgEnable = TRUE;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.fgAllocateBuffer = TRUE;
    rPid.ePcrMode = DMX_PCR_MODE_NONE;
    rPid.ePidType = DMX_PID_TYPE_PSI;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = DMX_DEFAULT_PSI_FIFO_SIZE;
    rPid.u2Pid = u2Pid;
    rPid.pfnNotify = _SectionHandler;
    rPid.pvNotifyTag = NULL;

    if (!DMX_SetPid(u1PidIndex, DMX_PID_FLAG_ALL, &rPid))
    {
        return -1;
    }

    return 0;
}


#ifdef CC_DMX_TS_LOOPBACK
//-----------------------------------------------------------------------------
/** _DMXCLI_Loopback
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_Loopback(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgEnable;

    if (i4Argc != 2)
    {
        Printf("%s [on|off]\n", aszArgv[0]);
        return 0;
    }

    if (x_strcmp(aszArgv[1], "on") == 0)
    {
        fgEnable = TRUE;
    }
    else if (x_strcmp(aszArgv[1], "off") == 0)
    {
        fgEnable = FALSE;
    }
    else
    {
        Printf("%s [on|off]\n", aszArgv[0]);
        return -1;
    }

    if (fgEnable)
    {
        if (!DMX_Loopback_Set())
        {
            return -1;
        }

        if (!DMX_Loopback_Enable(TRUE))
        {
            return -1;
        }
    }
    else
    {
        if (!DMX_Loopback_Enable(FALSE))
        {
            return -1;
        }

        if (!DMX_Loopback_Free())
        {
            return -1;
        }
    }

    return 0;
}
#endif  // CC_DMX_TS_LOOPBACK


#ifdef CC_DMX_DEBUG

//-----------------------------------------------------------------------------
/** _DMXCLI_SelInputType
 *  Command: init
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SelInputType(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_INPUT_TYPE_T rType;

    if (i4Argc != 2)
    {
        Printf("%s [bts|pts|ps|pvr_ts|mm|es|drm]\n", aszArgv[0]);
        Printf("    bts:    broadcast ts\n");
        Printf("    pts:    playback ts\n");
        Printf("    pvr_ts: PVR playback \n");
        Printf("    ps:     program stream\n");
        Printf("    mm:     Multimedia playback \n");
        Printf("    es:     elementary stream\n");
        Printf("    drm:    DRM\n");
        return 0;
    }

    if (x_strncmp(aszArgv[1], "bts", sizeof("bts"))==0)
    {
        rType = DMX_IN_BROADCAST_TS;
    }
    else if (x_strncmp(aszArgv[1], "pts", sizeof("pts"))==0)
    {
        rType = DMX_IN_PLAYBACK_TS;
    }
    else if (x_strncmp(aszArgv[1], "pvr_ts", sizeof("pvr_ts"))==0)
    {
        rType = DMX_IN_PVR_TS;
    }
    else if (x_strncmp(aszArgv[1], "mm", sizeof("mm"))==0)
    {
        rType = DMX_IN_PLAYBACK_MM;
    }
    else if (x_strncmp(aszArgv[1], "ps", sizeof("ps"))==0)
    {
        rType = DMX_IN_PLAYBACK_PS;
    }
    else if (x_strncmp(aszArgv[1], "es", sizeof("es"))==0)
    {
        rType = DMX_IN_PLAYBACK_ES;
    }
    else
    {
        Printf("%s [bts|pts|ps|pvr_ts|mm|es|drm]\n", aszArgv[0]);
        return -1;
    }

    if (!DMX_MUL_SetInstType(0, rType))
    {
        return -1;
    }

    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}


#ifdef DMX_MEASURE_PSI_TIME
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_PSIGetMaxTime(INT32 i4Argc, const CHAR** aszArgv)
{
    HAL_TIME_T rTime;

    DMX_PSI_GetMaxTime(&rTime);

    Printf("Section max time: %u.%06u\n", rTime.u4Seconds, rTime.u4Micros);

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_PSIResetMaxTime(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_ResetPSIMaxTime();

    Printf("Reset OK\n");

    return 0;
}
#endif  // DMX_MEASURE_PSI_TIME


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetBypassMode(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1TsIndex = _u1TsIndex, u1PacketSize = 188;
    BOOL fgRet;

    ASSERT(aszArgv != NULL);

    if (i4Argc > 1)
    {
        u1PacketSize = (UINT8)StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u1TsIndex = (UINT8)StrToInt(aszArgv[2]);
    }

    if ((i4Argc > 3) || (u1TsIndex > 1))
    {
        Printf("Usage: %s [packet_size] [TS]\n", aszArgv[0]);
    }

    Printf("Setting bypass mode: TS = %u, packet size = %u...\n",
           u1TsIndex, u1PacketSize);

    fgRet = DMX_SetBypassMode(u1TsIndex, u1PacketSize);
    Printf("%s!\n", fgRet ? "done" : "fail");

    return fgRet ? 0 : -1;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetErrorHandling(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Code;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 2)
    {
        Printf("Usage: %s handling_code\n", aszArgv[0]);
        return -1;
    }

    u4Code = (UINT32)StrToInt(aszArgv[1]);
    DMX_SetFramerErrorHandlingTable(u4Code);

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetCaOutput(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4BufStart, u4BufEnd, u4BufSize = 0, u4Threshold;
    BOOL fgIsPs;

    ASSERT(aszArgv != NULL);

    if (i4Argc == 4)
    {
        fgIsPs = (BOOL)StrToInt(aszArgv[1]) ? TRUE : FALSE;
        u4BufStart = (UINT32)StrToInt(aszArgv[2]);
        u4BufEnd = (UINT32)StrToInt(aszArgv[3]);
        if (!DMX_IsAligned(u4BufStart, DMX_CA_BUFFER_ALIGNMENT) ||
                !DMX_IsAligned(u4BufEnd, DMX_CA_BUFFER_ALIGNMENT))
        {
            Printf("CA output buffer addresses must be %u-byte alignment!\n",
                   DMX_CA_BUFFER_ALIGNMENT);
            return -1;
        }
    }
    else if (i4Argc == 3)
    {
        fgIsPs = (BOOL)StrToInt(aszArgv[1]) ? TRUE : FALSE;
        u4BufSize = (UINT32)StrToInt(aszArgv[2]);
        if (!DMX_IsAligned(u4BufSize, DMX_CA_BUFFER_ALIGNMENT))
        {
            Printf("CA output buffer size must be %u-byte alignment!\n",
                   DMX_CA_BUFFER_ALIGNMENT);
            return -1;
        }
        u4BufStart = (UINT32)BSP_AllocAlignedDmaMemory(u4BufSize,
                     DMX_CA_BUFFER_ALIGNMENT);
        if (u4BufStart == 0)
        {
            Printf("Out of memory!\n");
            return -1;
        }
        u4BufEnd = u4BufStart + u4BufSize;
    }
    else
    {
        Printf("Usage: %s is_ps(0,1) [buf_start buf_end]|[buf_size]\n",
               aszArgv[0]);
        return -1;
    }

    u4Threshold = u4BufSize / 4;    // FIXME

    if (DMX_SetCaOutputBuffer(TRUE, fgIsPs, u4BufStart, u4BufEnd, u4Threshold))
    {
        Printf("OK! Buffer: 0x%08x - 0x%08x\n", u4BufStart, u4BufEnd);
    }
    else
    {
        Printf("Fail\n");
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _CmdSetPidKeyIdx(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PID_T rPid;
    UINT8 u1Pidx, u1Keyx;

    if (i4Argc != 4)
    {
        Printf("%s [pidx] [key_index] [mode]\n", aszArgv[0]);
        return -1;
    }

    u1Pidx = StrToInt(aszArgv[1]);
    u1Keyx = StrToInt(aszArgv[2]);

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    rPid.eDescMode = (DMX_DESC_MODE_T)StrToInt(aszArgv[3]);
    rPid.u1KeyIndex = u1Keyx;                   // Start from index 8;

    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_DESC_MODE | DMX_PID_FLAG_KEY_INDEX, &rPid))
    {
        return FALSE;
    }

    Printf("Set Play PID OK\n");

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_ResetTest(INT32 i4Argc, const CHAR ** aszArgv)
{
    UINT32 u4Times = 100, i;

    ASSERT(aszArgv != NULL);

    if (i4Argc > 1)
    {
        u4Times = (UINT32)StrToInt(aszArgv[1]);
    }

    Printf("Reset %u times...\n", u4Times);

    for (i = 0; i < u4Times; i++)
    {
        IO_WRITE32(DEMUX0_BASE, REG_DMX_RESET, 0x80000000);

        while (1)
        {
            // Check if FTI is halted
            if (0x1 != ((IO_READ32(DEMUX0_BASE, REG_DMX_RESET) >> 28) & 0x3))
            {
                x_thread_delay(1);
            }
            else
            {
                break;
            }
        }

        Printf(".");
    }

    Printf("\nOK!\n");

    return 0;
}

//-----------------------------------------------------------------------------
static INT32 _DMXCLI_FrontEndEfuseTest(INT32 i4Argc, const CHAR ** aszArgv)
{
    Printf("DMX %s Support Extern TS -------------------\n", (DMX_Support_Tsin())?"":"not");
    return 0;
}

static INT32 _DMXCLI_ARIBEfuseTest(INT32 i4Argc, const CHAR ** aszArgv)
{
    Printf("DMX %s Support ARIB -------------------\n", (DMX_Support_ARIB())?"":"not");
    return 0;
}

#if 0
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_FrontEndTest(INT32 i4Argc, const CHAR ** aszArgv)
{
    UINT32 i, u4Count;

    u4Count = 1000;

    if (i4Argc == 2)
    {
        u4Count = StrToInt(aszArgv[1]);
    }

    for (i=0; i<1000; i++)
    {
        Printf("Loop %u -------------------\n", i);
        DMX_SetFrontEnd(0);
        x_thread_delay(5000);
        DMX_SetFrontEnd(2);
        x_thread_delay(500);
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_DramMeasure(INT32 i4Argc, const CHAR ** aszArgv)
{
    UINT32 u4Device, u4Times;

    if (i4Argc != 3)
    {
        Printf("%s device ms_time\n", aszArgv[0]);
        Printf("  device, 0:dmx, 1:ddi, 2:parser\n");
        return -1;
    }

    u4Device = StrToInt(aszArgv[1]);
    if (u4Device >= DMX_LOCAL_ARBITOR_NUM)
    {
        return -1;
    }

    u4Times = StrToInt(aszArgv[2]);

    if (!DMX_DRAMMeasure((DMX_LOCAL_ARBITOR_DEVICE_T)u4Device, u4Times))
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("OK\n");

    return 0;
}
#endif  // 0

#ifdef CC_DMX_SUPPORT_MULTI2
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _CmdSetMulti2CbcIV(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4IvHi, u4IvLo;
    UINT8 u1KeyIndex;

#ifdef CC_MT5880
    if (!IS_IC_5860_A_ES2())
        return -1;
#endif
    if (i4Argc != 4)
    {
        Printf("Usage: %s ,u1KeyIndex ,IV_hi ,IV_lo\n", aszArgv[0]);
        return -1;
    }

    u1KeyIndex = (UINT8)StrToInt(aszArgv[1]);
    u4IvHi = (UINT32)StrToInt(aszArgv[2]);
    u4IvLo = (UINT32)StrToInt(aszArgv[3]);

    DMX_SetMulti2CbcIV(u1KeyIndex,u4IvHi, u4IvLo);

    Printf("Set Multi2 CBC IV Done.\n");
    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _CmdSetMulti2OfbIV(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4IvHi, u4IvLo;
    UINT8 u1KeyIndex;

#ifdef CC_MT5880
    if (!IS_IC_5860_A_ES2())
        return -1;
#endif
    if (i4Argc != 4)
    {
        Printf("Usage: %s u1KeyIndex ,IV_hi ,IV_lo\n", aszArgv[0]);
        return -1;
    }

    u1KeyIndex = (UINT8)StrToInt(aszArgv[1]);
    u4IvHi = (UINT32)StrToInt(aszArgv[2]);
    u4IvLo = (UINT32)StrToInt(aszArgv[3]);

    DMX_SetMulti2OfbIV(u1KeyIndex, u4IvHi, u4IvLo);

    Printf("Set Multi2 Ofb IV Done.\n");
    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _CmdSetMulti2SysKey(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i;
    UINT32 au4Key[8];
    UINT8 u1KeyIndex;

#ifdef CC_MT5880
    if (!IS_IC_5860_A_ES2())
        return -1;
#endif
    if (i4Argc != 10)
    {
        Printf("Usage: %s ,u1KeyIndex ,k0, k1, k2, k3, k4, k5, k6 ,k7.\n", aszArgv[0]);
        return -1;
    }

    u1KeyIndex = (UINT8)StrToInt(aszArgv[1]);

    for (i = 0; i < 8; i++)
    {
        au4Key[i] = (UINT32)StrToInt(aszArgv[i + 2]);
    }

    DMX_SetMulti2SysKey(u1KeyIndex, au4Key);

    Printf("Set Multi2 Systerm Key Done.\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _CmdSetMulti2Config(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT16 u2Iteration;
    BOOL fgOfbFreeRun;
    UINT8 u1KeyIndex;

#ifdef CC_MT5880
    if (!IS_IC_5860_A_ES2())
        return -1;
#endif
    if (i4Argc != 4)
    {
        Printf("Usage: %s, u1KeyIndex, ofb_free_run(1,0) iteration(0-65535)\n",
               aszArgv[0]);
        return -1;
    }

    u1KeyIndex = (UINT8)StrToInt(aszArgv[1]);
    fgOfbFreeRun = (BOOL)StrToInt(aszArgv[2]);
    u2Iteration = (UINT16)StrToInt(aszArgv[3]);

    DMX_SetMulti2Config(u1KeyIndex, fgOfbFreeRun, u2Iteration);

    Printf("Set Multi2 Config Done.\n");

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _CmdSetMulti2Key(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1KeyIndex;
    BOOL fgEven;
    UINT32 au4Key[2];
    UINT8 i;

#ifdef CC_MT5880
    if (!IS_IC_5860_A_ES2())
        return -1;
#endif
    if (i4Argc != 5)
    {
        Printf("Usage: %s, u1KeyIndex, fgEven(1,0) K0, K1.\n",
               aszArgv[0]);
        return -1;
    }

    u1KeyIndex = (UINT8)StrToInt(aszArgv[1]);
    fgEven = (BOOL)StrToInt(aszArgv[2]);

    for (i = 0; i < 2; i++)
    {
        au4Key[i] = (UINT32)StrToInt(aszArgv[i + 3]);
    }

    if (!DMX_SetMulti2Key(u1KeyIndex, fgEven, au4Key))
    {
        return -1;
    }

    Printf("Set Multi2 Key Done.\n");
    return 0;
}

#endif   // CC_DMX_SUPPORT_MULTI2

//////////////////////////////////////////////////////
//  Audio PES
//////////////////////////////////////////////////////

static HANDLE_T _hAPESQueue = NULL_HANDLE;
static HANDLE_T _hAPESThread = NULL_HANDLE;
UINT8 u1Inst0;
UINT8 u1Pidx0, u1Pidx1;

static BOOL DMX_VideoPesHandler(const DMX_PES_MSG_T* prPes)
{
    return DMX_UpdateReadPointer(prPes->u1Pidx, prPes->u4Wp, prPes->u4FrameAddr);
}

static BOOL DMX_AudioPesHandler(const DMX_AUDIO_PES_T* prPes)
{
    return DMX_UpdateReadPointer(prPes->u1PidIndex, prPes->u4Wp, prPes->u4PesRp);
}

static void DMX_UpdateWp(UINT8 u1PidIndex, UINT32 u4Wp)
{

}

static VOID _APESThread(VOID* pvArg)
{
    UNUSED(pvArg);

    while (1)
    {
        static DMX_NOTIFY_INFO_PES_T rMsg;
        INT32 i4Ret;
        SIZE_T zMsgSize;
        UINT16 u2QueueIndex;

        zMsgSize = sizeof (rMsg);
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rMsg, &zMsgSize,
                                &_hAPESQueue, 1, X_MSGQ_OPTION_WAIT);

        DMX_PES_UnlockBuffer(u1Pidx1, rMsg.u1SerialNumber, rMsg.u4DataSize, rMsg.u4DataAddr);

    }   // while (1)
}


static BOOL _DmxCallback(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
                         UINT32 u4Data, const void* pvNotifyTag)
{
    // Check if PID is really active
    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    // Check notification code
    switch (eCode)
    {
    case DMX_NOTIFY_CODE_OVERFLOW:
        break;

    case DMX_NOTIFY_CODE_SCRAMBLE_STATE:
        break;

    case DMX_NOTIFY_CODE_PES:
        if (x_msg_q_send(_hAPESQueue, (VOID*)u4Data, sizeof (DMX_NOTIFY_INFO_PES_T),
                         128) != OSR_OK)
        {
            LOG(3, "APES queue full!\n");
        }
        break;

    default:
        // Unexpected notification
        ASSERT(0);
        break;
    }

    return TRUE;
}


static INT32 _DMXCLI_AudioPes(INT32 i4Argc, const CHAR ** aszArgv)
{
    DMX_PID_T rPid;
    DMX_DECODER_CALLBACKS_T rCallback;
    UINT32 u4Action;
    UINT32 u4Value;
    rCallback.pfnSendVideoPes = DMX_VideoPesHandler;
    rCallback.pfnSendAudioPes = DMX_AudioPesHandler;
    rCallback.pfnUpdateVideoWp = DMX_UpdateWp;

    u4Action = (UINT8)StrToInt(aszArgv[1]);
    u4Value = (UINT8)StrToInt(aszArgv[2]);

    if (u4Action == 0) // initial dtv
    {
        CLI_Parser("nim.go atsc");
        CLI_Parser("nim.freq 689000");
        CLI_Parser("fbm.i");
        CLI_Parser("vdec.i");
        CLI_Parser("d.i");
        CLI_Parser("d.sfe 0 0");

        // set the framer 0 pid for tuner 0
        u1Inst0 = DMX_MUL_GetAvailableInst(DMX_CONN_TYPE_TUNER_0);
        DMX_MUL_SetInstType(u1Inst0, DMX_IN_BROADCAST_TS);
        DMX_MUL_SetTSIdx(u1Inst0);
        DMX_MUL_SetDecoderCallbacks(u1Inst0, &rCallback);

        // Create picture queues
        if (x_msg_q_create(&_hAPESQueue, "FVR",
                           sizeof (DMX_NOTIFY_INFO_PES_T), 1024) != OSR_OK)
        {
            LOG(1, "Fail to create picture queue!\n");
            return FALSE;
        }

        // Create picture thread
        if (x_thread_create(&_hAPESThread, "FvrThread",
                            DMX_THREAD_STACK_SIZE, DMX_THREAD_PRIORITY, _APESThread, 0,
                            NULL) != OSR_OK)
        {
            LOG(1, "Fail to create DMX thread!\n");
            return FALSE;
        }
    }
    else if (u4Action == 1) // set audio ES
    {
        u1Pidx0 = DMX_MUL_GetAvailablePidx(u1Inst0);

        x_memset(&rPid, 0, sizeof(rPid));
        rPid.u1TsIndex = 0;
        rPid.fgEnable = TRUE;
        rPid.u2Pid = (UINT16)u4Value;
        rPid.u1SteerMode = DMX_STEER_TO_FTUP;
        rPid.ePidType = DMX_PID_TYPE_ES_AUDIO;
        rPid.fgAllocateBuffer = FALSE;
        rPid.u4BufAddr = (UINT32)BSP_AllocAlignedDmaMemory(0x100000,
                         64);
        rPid.u4BufSize = 0x100000;
        rPid.fgPrimary = TRUE;
        rPid.u1ChannelId = 0;

        if (!DMX_SetPid(u1Pidx0, DMX_PID_FLAG_ALL, &rPid))
        {
            Printf("fail to set pid\n");
        }
    }
    else if (u4Action == 2) // set audio PES
    {
        u1Pidx1 = DMX_MUL_GetAvailablePidx(u1Inst0);

        x_memset(&rPid, 0, sizeof(rPid));
        rPid.u1TsIndex = 0;
        rPid.fgEnable = TRUE;
        rPid.u2Pid = (UINT16)u4Value;
        rPid.u1SteerMode = DMX_STEER_TO_FTUP;
        rPid.ePidType = DMX_PID_TYPE_PES_AUDIO;
        rPid.fgAllocateBuffer = FALSE;
        rPid.u4BufAddr = (UINT32)BSP_AllocAlignedDmaMemory(0x100000,
                         64);
        rPid.u4BufSize = 0x100000;
        rPid.fgPrimary = FALSE;
        rPid.u1ChannelId = 0;
        rPid.pfnNotify = _DmxCallback;

        if (!DMX_SetPid(u1Pidx1, DMX_PID_FLAG_ALL, &rPid))
        {
            Printf("fail to set pid\n");
        }
    }
    else if (u4Action == 3) // free audio ES
    {
        x_memset(&rPid, 0, sizeof(rPid));
        rPid.fgEnable = FALSE;

        if (!DMX_SetPid(u1Pidx0, DMX_PID_FLAG_VALID, &rPid))
        {
            Printf("fail to set pid\n");
        }

        if (!DMX_FreePid(u1Pidx0))
        {
            Printf("fail to free pid\n");
        }

        if (!DMX_MUL_FreePidx(u1Pidx0))
        {
            Printf("fail to free mul pid\n");
        }
    }
    else if (u4Action == 4) // free audio PES
    {
        x_memset(&rPid, 0, sizeof(rPid));
        rPid.fgEnable = FALSE;

        if (!DMX_SetPid(u1Pidx1, DMX_PID_FLAG_VALID, &rPid))
        {
            Printf("fail to set pid\n");
        }

        if (!DMX_FreePid(u1Pidx1))
        {
            Printf("fail to free pid\n");
        }

        if (!DMX_MUL_FreePidx(u1Pidx1))
        {
            Printf("fail to free mul pid\n");
        }
    }

    return 0;
}

//////////////////////////////////////////////////////
//  Audio PES
//////////////////////////////////////////////////////


#endif  // CC_DMX_DEBUG


static INT32 _DMXCLI_PSW(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1NewPidx, u1OldPidx;
    UINT16 u2Pid;
    UINT8 u1Inst;

    u1OldPidx = (UINT8)StrToInt(aszArgv[1]);
    u2Pid = (UINT16)StrToInt(aszArgv[2]);

    u1Inst = DMX_MUL_GetAvailableInst(DMX_CONN_TYPE_TUNER_0);
    u1NewPidx = DMX_MUL_GetAvailablePidx(u1Inst);

    if (!DMX_SetSeamlessPID(u1NewPidx, u1OldPidx, u2Pid))
    {
        Printf("DMX_SetSeamlessPID fail\n");
        return -1;
    }

    Printf("DMX_SetSeamlessPID OK\n");

    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}

static INT32 _DMXCLI_FSP(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1OldPidx;

    u1OldPidx = (UINT8)StrToInt(aszArgv[1]);

    if (!DMX_FreeSwitchPid(u1OldPidx))
    {
        Printf("DMX_FreeSwitchPid fail\n");
        return -1;
    }

    if (!DMX_MUL_FreePidx(u1OldPidx))
    {
        Printf("DMX_MUL_FreePidx fail\n");
        return -1;
    }

    Printf("DMX_FreeSwitchPid OK\n");

    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}


//-----------------------------------------------------------------------------
static CLI_EXEC_T _arDmxCmdTbl[] =
{
    {
        "init", "i", _DMXCLI_Init, NULL, "", CLI_SUPERVISOR
    },

    {
        "setfe", "sfe", _DMXCLI_SetFrontEnd, NULL, "", CLI_SUPERVISOR
    },

    {
        "setframer", "sframer", _DMXCLI_SetFramer, NULL, "", CLI_SUPERVISOR
    },

    {
        "resetframer", "rframer", _DMXCLI_ResetFramer, NULL, "", CLI_SUPERVISOR
    },

    {
        "setpid", "sp", _DMXCLI_SetPid, NULL, "", CLI_SUPERVISOR
    },

    {
        "freepid", "fp", _DMXCLI_FreePid, NULL, "", CLI_SUPERVISOR
    },

    {
        "getpid", "gp", _DMXCLI_GetPid, NULL, "", CLI_SUPERVISOR
    },

    {
        "getpidmaxsize", "gpm", _DMXCLI_GetPidMaxSize, NULL, "", CLI_SUPERVISOR
    },

    {
        "enablepid", "ep", _DMXCLI_EnablePid, NULL, "", CLI_SUPERVISOR
    },

    {
        "setpcr", "spcr", _DMXCLI_SetPcr, NULL, "", CLI_SUPERVISOR
    },

    {
        "setpsipid", "spp", _DMXCLI_SetPsiPid, NULL, "", CLI_SUPERVISOR
    },

    {
        "setfilter", "sf", _DMXCLI_SetFilter, NULL, "", CLI_SUPERVISOR
    },

    {
        "getfilter", "gf", _DMXCLI_GetFilter, NULL, "", CLI_SUPERVISOR
    },

    {
        "getgenericfilter", "ggf", _DMXCLI_GetGenericFilter, NULL, "", CLI_SUPERVISOR
    },

    {
        "getcounters", "gc", _DMXCLI_GetCounters, NULL, "", CLI_SUPERVISOR
    },

    {
        "resetcounters", "rc", _DMXCLI_ResetCounters, NULL, "", CLI_SUPERVISOR
    },

    {
        "query", "q", _DMXCLI_Query, NULL, "", CLI_GUEST
    },

    {
        "MULInstQuery", "mulinst", _DMXCLI_MULInstQuery, NULL, "Show multi inst info", CLI_SUPERVISOR
    },

    {
        "MULPidQuery", "mulpid", _DMXCLI_MULPidQuery, NULL, "Show multi pid info", CLI_SUPERVISOR
    },

    {
        "packet", "pkt", _DMXCLI_RcvPktCount, NULL, "", CLI_GUEST
    },

    {
        "Tsindex", "settsidx", _DMXCLI_SetTsIndex, NULL, "Set ts index", CLI_SUPERVISOR
    },

    {
        "version", "ver", _DMXCLI_Version, NULL, "Get version", CLI_SUPERVISOR
    },

    {
        "EfuseStatus", "efuse", _DMXCLI_GetEfuseStatus, NULL, "", CLI_SUPERVISOR
    },

    {
        "power", "power", _DMXCLI_Power, NULL, "Set power on off", CLI_SUPERVISOR
    },

    {
        "CheckAW", "aw", _DMXCLI_CheckAW, NULL, "Check AW support", CLI_SUPERVISOR
    },

    {
        "setpsihandler", "sph", _DMXCLI_SetPsiPidCallback, NULL, "", CLI_SUPERVISOR
    },

    {
        "Test pat", "pat", _DMXCLI_TestPATSection, NULL, "", CLI_SUPERVISOR
    },

    {
        "Freee pat", "freepat", _DMXCLI_FreePATSection, NULL, "", CLI_SUPERVISOR
    },

    {
        "capture", "cap", _DMXCLI_Capture, NULL, "Capture test", CLI_SUPERVISOR
    },

    {
        "capturebuf", "cbuf", _DMXCLI_CaptureAllocBuf, NULL, "cbuf [size]", CLI_SUPERVISOR
    },

    {
        "captureinfo", "cinfo", _DMXCLI_CaptureInfo, NULL, "cinfo", CLI_SUPERVISOR
    },

    {
        "cccaptureinit", "ccci", _DMXCLI_ChangeChannelCaptureInit, NULL, "CC capture init", CLI_SUPERVISOR
    },

    {
        "cccapture", "ccc", _DMXCLI_ChangeChannelCapture, NULL, "CC Capture event", CLI_SUPERVISOR
    },

    {
        "errlevel", "el", _DMXCLI_SetErrLevel, NULL, "el", CLI_SUPERVISOR
    },

    {
        "scrtype", "scrt", _DMXCLI_SetScrambleScheme, NULL, "scrt", CLI_SUPERVISOR
    },

    {
        "setvideotype", "svt", _DMXCLI_SetVideoType, NULL, "", CLI_SUPERVISOR
    },

    {
        "getscramblestate", "gss", _DMXCLI_GetScrambleState, NULL, "", CLI_SUPERVISOR
    },
			
#if defined(CC_MT5399) || defined(CC_MT5890)

    {
        "FramerErrorTable", "sfet", _DMXCLI_SetFramerPacketErrorHandling, NULL, "FramerErrorTable", CLI_SUPERVISOR
    },

    {
        "SetDbmInputSource", "sdis", _DMXCLI_SetDbmInputSource, NULL, "SetDbmInputSource", CLI_SUPERVISOR
    },
#endif

    {
        "pidswitch", "psw", _DMXCLI_PSW, NULL, "", CLI_SUPERVISOR
    },

    {
        "freeswitchpid", "fsp", _DMXCLI_FSP, NULL, "", CLI_SUPERVISOR
    },

#ifdef TIME_SHIFT_SUPPORT
    {
        "timeshiftq", "tsq", _DMXCLI_TimeShiftQuery, NULL, "tsq", CLI_GUEST
    },

    {
        "timeshiftresetc", "tsrc", _DMXCLI_TimeShiftResetCounter, NULL, "tsrc", CLI_SUPERVISOR
    },
#endif  // TIME_SHIFT_SUPPORT

#ifdef CC_DMX_TS_LOOPBACK
    {
        "loopback", "lp", _DMXCLI_Loopback, NULL, "lp", CLI_SUPERVISOR
    },
#endif // CC_DMX_TS_LOOPBACK

#ifdef CC_DMX_DEBUG
    {
        "selinput", "si", _DMXCLI_SelInputType, NULL, "", CLI_SUPERVISOR
    },

#ifdef DMX_MEASURE_PSI_TIME
    {
        "psigettime", "psigt", _DMXCLI_PSIGetMaxTime, NULL, "", CLI_SUPERVISOR
    },

    {
        "psiresettime", "psirt", _DMXCLI_PSIResetMaxTime, NULL, "", CLI_SUPERVISOR
    },
#endif  // DMX_MEASURE_PSI_TIME

    {
        "setbypassmode", "sb", _DMXCLI_SetBypassMode, NULL, "", CLI_SUPERVISOR
    },

    {
        "seterrorhandling", "seh", _DMXCLI_SetErrorHandling, NULL, "", CLI_SUPERVISOR
    },

    {
        "setcaoutput", "sco", _DMXCLI_SetCaOutput, NULL, "", CLI_SUPERVISOR
    },

    {
        "setpidkeyidx", "skeyidx", _CmdSetPidKeyIdx, NULL, "skeyidx [pidx] [key_idx]", CLI_SUPERVISOR
    },

    {
        "resettest", "rt", _DMXCLI_ResetTest, NULL, "Reset test", CLI_SUPERVISOR
    },

//    {
//        "drammeasure", "dm", _DMXCLI_DramMeasure, NULL, "dram measure", CLI_SUPERVISOR
//    },

//    {
//        "fetest", "fet", _DMXCLI_FrontEndTest, NULL, "Front end test", CLI_SUPERVISOR
//    },

    {
        "fefusetest", "fet", _DMXCLI_FrontEndEfuseTest, NULL, "Front end efuse test", CLI_SUPERVISOR
    },

    {
        "aefusetest", "aet", _DMXCLI_ARIBEfuseTest, NULL, "ARIB efuse test", CLI_SUPERVISOR
    },

    {
        "apes", "apes", _DMXCLI_AudioPes, NULL, "", CLI_SUPERVISOR
    },
#ifdef CC_DMX_SUPPORT_MULTI2
    {
        "setm2config", "smcfg", _CmdSetMulti2Config, NULL, "", CLI_SUPERVISOR
    },

    {
        "setm2cbciv", "smciv", _CmdSetMulti2CbcIV, NULL, "", CLI_SUPERVISOR
    },

    {
        "setm2ofbiv", "smoiv", _CmdSetMulti2OfbIV, NULL, "", CLI_SUPERVISOR
    },

    {
        "setm2syskey", "smsk", _CmdSetMulti2SysKey, NULL, "", CLI_SUPERVISOR
    },
    {
        "setm2key", "smk", _CmdSetMulti2Key, NULL, "", CLI_SUPERVISOR
    },

#endif  // CC_DMX_SUPPORT_MULTI2
#endif  // CC_DMX_DEBUG

// These functions exist only in SLT mode

    {
        "diag", "diag", _DMXCLI_Diag, NULL, "Self diagnostic test", CLI_SUPERVISOR
    },

    {
        "jchip", "jchip", _DMXCLI_JChipDiag, NULL, "J-chip functionality test", CLI_SUPERVISOR
    },

#ifdef CC_DMX_ERR_RECOVERY_TEST
    {
        "panictest", "pnt", _DMXCLI_PanicTest, NULL, "pnt", CLI_SUPERVISOR
    },

    {
        "monpanictest", "mpnt", _DMXCLI_MonPanicTest, NULL, "", CLI_SUPERVISOR
    },

    {
        "overflowtest", "ot", DMXCLI_FVROverflow, NULL, "", CLI_SUPERVISOR
    },
#endif // CC_DMX_ERR_RECOVERY_TEST

    {
        "enptslog", "enptslog", _DMXCLI_EnablePtsInfo, NULL, "", CLI_GUEST
    },

    {
        "ptsdrift", "ptsdrift", _DMXCLI_PtsDriftInfo, NULL, "", CLI_SUPERVISOR
    },

    {
        "printtraces", "tr", _DMXCLI_PrintTraceLogFiles, NULL, "Print traces", CLI_SUPERVISOR
    },

#ifdef CC_DMX_TRACK_ERROR_INTS
    {
        "debughwerr", "dhwerr", _DMXCLI_DebugHWError, NULL, "Debug DMX hardware errors", CLI_SUPERVISOR
    },
#endif // CC_DMX_TRACK_ERROR_INTS


#ifndef NDEBUG
    {
        "debugmask", "dl", _DMXCLI_DebugMask, NULL, "", CLI_SUPERVISOR
    },
#endif
#ifdef CC_EIT_SECTION_FILTER
    {
        "eitflt", "eitf", _DMXCLI_EITFilter, NULL, "Set EIT filter", CLI_SUPERVISOR
    },
#endif

    {
        "genkey", "genkey", _DMXCLI_GenerateKey, NULL, "generate sample key", CLI_GUEST
    },


    // LOG macro support
    LINT_SAVE_AND_DISABLE
    CLIMOD_DEBUG_CLIENTRY(DMX),
    LINT_RESTORE

    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }

};


LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(Dmx)
{
    "dmx", "d", NULL, _arDmxCmdTbl, "Demux commands", CLI_GUEST

};
LINT_RESTORE


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

#if 0
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
#ifdef __MW_CLI_DEF__
CLI_EXEC_T* GetDmxMwCmdTbl(void)
#else
CLI_EXEC_T* GetDmxCmdTbl(void)
#endif  // __MW_CLI_DEF__
{
    return _arDmxCmd;
}

#endif  // 0
