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
#ifndef CC_MTK_LOADER
#include "x_hal_arm.h"
#include "x_assert.h"
#include "x_serial.h"
#include "x_typedef.h"
#include "x_stl_lib.h"
#include "x_os.h"
#include "x_timer.h"
#define DEFINE_IS_LOG CLI_IsLog
#include "x_debug.h"
#include "x_util.h"
#include "x_driver_os.h"
#include "x_lint.h"
#include "drv_common.h"

//lint -save
//lint -e960 cannot use #/## in maro many times.
//lint -e950 use __align(16)
//lint -e818 bother to ask ass const in function parameter.
//lint -e718 va_list...
//lint -e645 variable might not initialize, but it's stupid.
//lint -e564 a variable appears too many time in expression.
//lint -e530 va_list not initial.
//lint -e522 I don't know why cannot use function pointer.

#ifndef CC_DRIVER_PROGRAM
#define DEFAULT_LEVEL   0
#else
#ifdef __KERNEL__
#define DEFAULT_LEVEL   0
#else                     // defined(CC_DRIVER_PROGRAM) && !defined(__KERNEL__)
#define DEFAULT_LEVEL   0 /* raise default debug level in drv_build */
#endif
#endif

#define LOCAL_BUFSZ         0x80

/***********************************************************************/

#define LOG_LEVEL_FUNCTIONS(mod, level)                                 \
                                                                        \
static INT32 _i4##mod##LogLvl = (level);                                \
                                                                        \
INT32 mod##_i4LogLvlQuerySet(INT32 const *pi4Data)                      \
{                                                                       \
    if (pi4Data) {                                                      \
        _i4##mod##LogLvl = *pi4Data;                                    \
    }                                                                   \
    return _i4##mod##LogLvl;                                            \
}                                                                       \
                                                                        \
INT32 mod##_IsLog(INT32 i4Level, const CHAR *szFmt, ...)                \
{                                                                       \
    VA_LIST t_ap;                                                       \
                                                                        \
    if (_fgLog && ((i4Level <= (_i4##mod##LogLvl & 0x0ff))              \
        || (i4Level & 0x0ff00 & _i4##mod##LogLvl) || (i4Level & _S))) { \
        VA_START(t_ap, szFmt);                                          \
        i4Level = _UTIL_SetLog(_i4##mod##LogLvl, #mod, szFmt, t_ap,     \
        i4Level);                                                       \
        VA_END(t_ap);                                                   \
        return i4Level;                                                 \
    }                                                                   \
    return 0;                                                           \
}                                                                       \
                                                                        \
INT32 mod##_vLog(INT32 i4Level, const CHAR *szFmt, VA_LIST t_ap)        \
{                                                                       \
    if (_fgLog && ((i4Level <= (_i4##mod##LogLvl & 0x0ff))              \
        || (i4Level & 0x0ff00 & _i4##mod##LogLvl) || (i4Level & _S))) { \
        i4Level = _UTIL_SetLog(_i4##mod##LogLvl, #mod, szFmt, t_ap,     \
        i4Level);                                                       \
        return i4Level;                                                 \
    }                                                                   \
    return 0;                                                           \
}                                                                       \


static BOOL _fgLog = TRUE;

#ifdef DRV_DBG_LOG_TO_FILE 
extern INT32 x_log_to_drv_file_query(CHAR** ppBuffer,UINT32 *p_ui4Len);
extern  VOID x_log_to_drv_file_output_fct(const CHAR* ps_stmt);
#endif

BOOL UTIL_SetLogFlag(BOOL fgSet)
{
    BOOL fgRet;

    fgRet = _fgLog;
    _fgLog = fgSet;
    return fgRet;
}

UINT8 *UTILCLI_LogLevelText(INT32 i4LogLevel)
{
    static UINT8 _au1LogLevelText[LOCAL_BUFSZ];

    x_snprintf((CHAR *)_au1LogLevelText, LOCAL_BUFSZ-1, "%d (%s, %s, %s, %s, %s, %s, %s, %s)", i4LogLevel & 0x0ff,
            ((i4LogLevel & _R) ? "+R" : "-R"),
            ((i4LogLevel & _W) ? "+W" : "-W"),
            ((i4LogLevel & _B) ? "+B" : "-B"),
            ((i4LogLevel & _I) ? "+I" : "-I"),
            ((i4LogLevel & _M) ? "+M" : "-M"),
            ((i4LogLevel & _T) ? "+T" : "-T"),
            ((i4LogLevel & _L) ? "+L" : "-L"),
            ((i4LogLevel & _X) ? "+X" : "-X"));
    return _au1LogLevelText;
}

void UTILCLI_DebugLevel(FN_LQS_T i4LogLvlQuerySet, INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i, i4DbgLvl;

    ASSERT(szArgv!=NULL);
    ASSERT(i4LogLvlQuerySet);
    for (i=1; i<i4Argc; i++) {
        if ((szArgv[i][0] == '+') || (szArgv[i][0] == '-')) {
            switch(szArgv[i][1]) {
            case 'R': case 'r':
                i4DbgLvl = i4LogLvlQuerySet(NULL);
                if (szArgv[i][0] == '+') {
                        if ((i4DbgLvl & _R) == 0) { i4DbgLvl += _R; }
                } else {
                        if ((i4DbgLvl & _R) != 0) { i4DbgLvl -= _R; }
                }
                i4LogLvlQuerySet(&i4DbgLvl);
                break;
            case 'W': case 'w':
                i4DbgLvl = i4LogLvlQuerySet(NULL);
                if (szArgv[i][0] == '+') {
                        if ((i4DbgLvl & _W) == 0) { i4DbgLvl += _W; }
                } else {
                        if ((i4DbgLvl & _W) != 0) { i4DbgLvl -= _W; }
                }
                i4LogLvlQuerySet(&i4DbgLvl);
                break;
            case 'B': case 'b':
                i4DbgLvl = i4LogLvlQuerySet(NULL);
                if (szArgv[i][0] == '+') {
                        if ((i4DbgLvl & _B) == 0) { i4DbgLvl += _B; }
                } else {
                        if ((i4DbgLvl & _B) != 0) { i4DbgLvl -= _B; }
                }
                i4LogLvlQuerySet(&i4DbgLvl);
                break;
            case 'I': case 'i':
                i4DbgLvl = i4LogLvlQuerySet(NULL);
                if (szArgv[i][0] == '+') {
                        if ((i4DbgLvl & _I) == 0) { i4DbgLvl += _I; }
                } else {
                        if ((i4DbgLvl & _I) != 0) { i4DbgLvl -= _I; }
                }
                i4LogLvlQuerySet(&i4DbgLvl);
                break;
            case 'M': case 'm':
                i4DbgLvl = i4LogLvlQuerySet(NULL);
                if (szArgv[i][0] == '+') {
                        if ((i4DbgLvl & _M) == 0) { i4DbgLvl += _M; }
                } else {
                        if ((i4DbgLvl & _M) != 0) { i4DbgLvl -= _M; }
                }
                i4LogLvlQuerySet(&i4DbgLvl);
                break;
            case 'T': case 't':
                i4DbgLvl = i4LogLvlQuerySet(NULL);
                if (szArgv[i][0] == '+') {
                        if ((i4DbgLvl & _T) == 0) { i4DbgLvl += _T; }
                } else {
                        if ((i4DbgLvl & _T) != 0) { i4DbgLvl -= _T; }
                }
                i4LogLvlQuerySet(&i4DbgLvl);
                break;
            case 'L': case 'l':
                i4DbgLvl = i4LogLvlQuerySet(NULL);
                if (szArgv[i][0] == '+') {
                        if ((i4DbgLvl & _L) == 0) { i4DbgLvl += _L; }
                } else {
                        if ((i4DbgLvl & _L) != 0) { i4DbgLvl -= _L; }
                }
                i4LogLvlQuerySet(&i4DbgLvl);
                break;
            case 'X': case 'x':
                i4DbgLvl = i4LogLvlQuerySet(NULL);
                if (szArgv[i][0] == '+') {
                        if ((i4DbgLvl & _X) == 0) { i4DbgLvl += _X; }
                } else {
                        if ((i4DbgLvl & _X) != 0) { i4DbgLvl -= _X; }
                }
                i4LogLvlQuerySet(&i4DbgLvl);
                break;
            default:
                Printf("%s is not support.\n", szArgv[i]);
                break;
            }
        } else {
            if ((szArgv[i][0] >= '0') && (szArgv[i][0] <='9')) {
                i4DbgLvl = (StrToInt(szArgv[i]) & 0x0ff) | (i4LogLvlQuerySet(NULL) & 0xffff00);
                i4LogLvlQuerySet(&i4DbgLvl);
            }
        }
    }
}

/******************************************************************************
*****    Memory buffer message mechanism.
******************************************************************************/
#ifdef CC_MINI_DRIVER
#define LOGBUF_SIZE         0x1000
#else /* CC_MINI_DRIVER */
#define LOGBUF_SIZE         0x2000
#endif /* CC_MINI_DRIVER */
#define LOGBUF_MASK         (LOGBUF_SIZE - 1)
#define LOGFULL_MARK        (0xff)

static UINT8 _au1LogBuf[LOGBUF_SIZE];
static volatile INT32 _i4BufIn = 0;
static volatile INT32 _i4BufOut = 0;
static UINT32 _u4Discard = 0;
static UINT32 _u4TotalIn = 0;
static UINT32 _u4TotalOut = 0;
static BOOL _fgPTInit = FALSE;

#ifndef CC_MINI_DRIVER
#define MAX_SLOG_NUM        0x100
//static CHAR _aszSLogBuf[MAX_SLOG_NUM][LOCAL_BUFSZ];
static CHAR *_aszSLogBuf;
static UINT32 _u4SLogIndex = 0;
#endif /* CC_MINI_DRIVER */

static HANDLE_T _hPThread;

#define LOGBUF_DATASZ()             (((_i4BufIn - _i4BufOut) >= 0) ? (_i4BufIn - _i4BufOut) : ((LOGBUF_SIZE + _i4BufIn) - _i4BufOut))
#define LOGBUF_FREESZ()             ((LOGBUF_SIZE - LOGBUF_DATASZ()) - 1)
#define LOGBUF_IDXADD(idx,off)      (idx = (((idx) + (off)) & LOGBUF_MASK))
#define LOGBUF_LASTDATA()           ((_i4BufIn == 0) ? (_au1LogBuf[LOGBUF_MASK]) : _au1LogBuf[(_i4BufIn - 1)])
#define LOGBUF_IDXSET(idx,val)      (idx = ((val) & LOGBUF_MASK))
#define LOGBUF_EMPTY()              (_i4BufIn == _i4BufOut)
#define LOGBUF_DATA(idx)            (_au1LogBuf[(idx)])

void UTIL_LogFlush()
{
    UINT8 u1Tmp;

    while (!LOGBUF_EMPTY()) {
        u1Tmp = LOGBUF_DATA(_i4BufOut);
        LOGBUF_IDXADD(_i4BufOut, 1);
        outbyte((CHAR)u1Tmp);
        _u4TotalOut++;
    }
    UNUSED(_u4Discard);
    UNUSED(_u4TotalIn);
    UNUSED(_u4TotalOut);
}

static void _LogThreadBody(void)
{
    UINT32 u4State;
    UINT8 u1Tmp;
    INT32 i4Ret, fgRun;
    
    #ifdef DRV_DBG_LOG_TO_FILE 
    UINT32 ui4_loop = 0;
    CHAR* pStr = NULL;
    UINT32 ui4_len;
    #endif
    
    UNUSED(i4Ret);
    fgRun = 1;
    while (fgRun)
    {
        #ifdef DRV_DBG_LOG_TO_FILE 
        if(x_log_to_drv_file_query(&pStr,&ui4_len))
        {
            ASSERT(pStr!=NULL);
            u4State = HalCriticalStart();
            ui4_loop = 0;
            x_memset(pStr, 0, 0x200);
            HalCriticalEnd(u4State);
            
            while (!LOGBUF_EMPTY())
            {
                u4State = HalCriticalStart();
                u1Tmp = LOGBUF_DATA(_i4BufOut);
                LOGBUF_IDXADD(_i4BufOut, 1);
                HalCriticalEnd(u4State);
                if (u1Tmp == LOGFULL_MARK)
                {
                    u4State = HalCriticalStart();
                    Printf("\n\n[!!!!!LOG FULL!!!!!]\n\n");
                    HalCriticalEnd(u4State);
                }
                else
                {
                    u4State = HalCriticalStart();
                    pStr[ui4_loop] = (CHAR)u1Tmp;
                    ui4_loop++;
                    HalCriticalEnd(u4State);
                }
                _u4TotalOut++;
            }
            ASSERT(ui4_loop<=ui4_len);
            x_log_to_drv_file_output_fct(pStr);
        }
        else
        #endif
        {
            while (!LOGBUF_EMPTY())
            {
                u4State = HalCriticalStart();
                u1Tmp = LOGBUF_DATA(_i4BufOut);
                LOGBUF_IDXADD(_i4BufOut, 1);
                HalCriticalEnd(u4State);
                if (u1Tmp == LOGFULL_MARK)
                {
                    u4State = HalCriticalStart();
                    Printf("\n\n[!!!!!LOG FULL!!!!!]\n\n");
                    HalCriticalEnd(u4State);
                }
                else
                {
                    outbyte((CHAR)u1Tmp);
                }
                _u4TotalOut++;
            }
        }

        /* If context switch here, it might cause delay print. XXX fix me!! */
        x_thread_suspend();
    }
}

static void _PrintThread(void *pvDummy)
{
    UNUSED(pvDummy);
    _LogThreadBody();
}

static INT32 _UTIL_Puts(CHAR *szBuf)
{
    INT32 i4Ret;

    ASSERT(szBuf!=NULL);
    i4Ret = 0;
    while (szBuf[i4Ret]) {
        outbyte((CHAR)szBuf[i4Ret]);
        i4Ret++;
    }
    return i4Ret;
}

static INT32 _UTIL_vPuts(const CHAR *szFmt, VA_LIST t_ap)
{
    INT32 i4Ret;
    CHAR szBuf[LOCAL_BUFSZ];

    i4Ret = x_vsnprintf(szBuf, LOCAL_BUFSZ-1, szFmt, t_ap);
    LINT_SUPPRESS_NEXT_EXPRESSION(534);
    _UTIL_Puts(szBuf);
    return i4Ret;
}

INT32 UTIL_LogThreadInit()
{
    INT32 i4Ret;

    if (!_fgPTInit) {
        i4Ret = x_thread_create(&_hPThread, LOG_PRINT_NAME, LOG_PRINT_STACK_SZ,
                                LOG_PRINT_PRIORITY, _PrintThread, 0, NULL);
        if (i4Ret != OSR_OK)
        {
            ASSERT(0);
            LINT_SUPPRESS_NEXT_EXPRESSION(527);
            _fgPTInit = FALSE;
            LINT_SUPPRESS_NEXT_EXPRESSION(534);
            _UTIL_Puts("LogThread Init failed.\n");
            return -1;
        }
        _fgPTInit = TRUE;
    }
    return OSR_OK;
}

static INT32 _UTIL_Buf2Mem(CHAR *szBuf, INT32 i4Len)
{
    INT32 i4Ret;

    ASSERT(szBuf!=NULL);
    if ((szBuf[0] == (CHAR)LOGFULL_MARK) && (i4Len==1) &&
        (LOGBUF_FREESZ() < (LOGBUF_SIZE >> 1)) &&
        (LOGBUF_LASTDATA() == LOGFULL_MARK)) {
        /* There is already a log full mark. */
        return 0;
    }

    if ((LOGBUF_SIZE - _i4BufIn) < i4Len) {
        ASSERT((LOGBUF_SIZE - _i4BufIn) < LOGBUF_SIZE);
        ASSERT((LOGBUF_SIZE - _i4BufIn) > 0);
        LINT_SUPPRESS_NEXT_EXPRESSION(534);
        x_strncpy((CHAR *)_au1LogBuf+_i4BufIn, szBuf, (UINT32)(LOGBUF_SIZE - _i4BufIn));
        ASSERT((i4Len - (LOGBUF_SIZE - _i4BufIn)) < LOGBUF_SIZE);
        ASSERT((i4Len - (LOGBUF_SIZE - _i4BufIn)) > 0);
        LINT_SUPPRESS_NEXT_EXPRESSION(534);
        x_strncpy((CHAR *)_au1LogBuf, szBuf + (LOGBUF_SIZE - _i4BufIn),
                        (UINT32)(i4Len - (LOGBUF_SIZE - _i4BufIn)));
    } else {
        ASSERT(i4Len < LOGBUF_SIZE);
        ASSERT(i4Len > 0);
        LINT_SUPPRESS_NEXT_EXPRESSION(534);
        x_strncpy((CHAR *)_au1LogBuf+_i4BufIn, szBuf, (UINT32)i4Len);
    }
    i4Ret = LOGBUF_FREESZ();
    LOGBUF_IDXADD(_i4BufIn, i4Len);
    if (i4Ret < i4Len) {
        LOGBUF_IDXSET(_i4BufOut, (_i4BufIn + 1));
        _u4Discard++;
    }
    _u4TotalIn += (UINT32)i4Len;

    return i4Len;
}

static INT32 _UTIL_sLog(CHAR *szStr, INT32 i4Len)
{
    UINT32 u4State;

    if (!_fgPTInit) {
        LINT_SUPPRESS_NEXT_EXPRESSION(534);
        _UTIL_Puts(szStr);
        return i4Len;
    }

    u4State = HalCriticalStart();
    LINT_SUPPRESS_NEXT_EXPRESSION(534);
    _UTIL_Buf2Mem(szStr, i4Len);
    HalCriticalEnd(u4State);

    x_thread_resume(_hPThread);
    return i4Len;
}

INT32 UTIL_vLog(const CHAR *szFmt, VA_LIST t_ap)
{
    INT32 i4Ret, i4Len;
    CHAR szStr[LOCAL_BUFSZ];

    i4Len = x_vsnprintf((CHAR *)szStr, LOCAL_BUFSZ-1, szFmt, t_ap);
    ASSERT(i4Len < LOCAL_BUFSZ);
    i4Ret = _UTIL_sLog((CHAR *)szStr, i4Len);
    return i4Ret;
}

INT32 UTIL_Log(const CHAR *szFmt, ...)
{
    VA_LIST t_ap;
    INT32 i4Ret, i4Len;
    CHAR szStr[LOCAL_BUFSZ];

    VA_START(t_ap, szFmt);
    i4Len = x_vsnprintf((CHAR *)szStr, LOCAL_BUFSZ-1, szFmt, t_ap);
    ASSERT(i4Len < LOCAL_BUFSZ);
    VA_END(t_ap);
    i4Ret = _UTIL_sLog((CHAR *)szStr, i4Len);
    return i4Ret;
}

#ifndef CC_MINI_DRIVER
void UTIL_FlushSLog(void)
{
    UINT32 u4Idx = 0, u4State = 0;
    if (_u4SLogIndex==0)
        return;

    u4State = HalCriticalStart();
    for (; (u4Idx<_u4SLogIndex) && (u4Idx<MAX_SLOG_NUM); u4Idx++)
        Printf("%s", _aszSLogBuf+u4Idx*LOCAL_BUFSZ*sizeof(char));
    _u4SLogIndex = 0;
    HalCriticalEnd(u4State);
}
#endif /* CC_MINI_DRIVER */

UINT32 _UTIL_Current_Task_Pointer(void)
{
    UINT32 u4Id, u4Tag;

    UNUSED(u4Tag);
    VERIFY(OSR_DRV_OK == x_os_drv_thread_self((VOID **)&u4Id, (VOID **)&u4Tag));
    return u4Id;
}

static INT32 _UTIL_SetLog(INT32 i4ModLvl, CHAR *szMod, const CHAR *szFmt, VA_LIST t_ap, INT32 i4Level)
{
    INT32 i4Ret, iIdLen = 0, iTimeLen = 0, iModLen = 0, i4Len;
    HAL_TIME_T rTime;
    UINT32 u4State;
    CHAR szTId[12];
    CHAR szTime[24];
    CHAR szStr[LOCAL_BUFSZ];

    if ((!_fgPTInit) || ((i4ModLvl & _L)==0)) { /* to Serial Console directly. */
        i4Ret = 0;
        if (i4ModLvl & _M) {
            i4Ret += Printf("[%s]", szMod);
        }
        if (i4ModLvl & _T) {
            i4Ret += Printf("[%08x]", _UTIL_Current_Task_Pointer());
        }
        if (i4ModLvl & _X) {
            HAL_GetTime(&rTime);
            i4Ret += Printf("[%d.%06d]", rTime.u4Seconds, rTime.u4Micros);
        }
        i4Ret += _UTIL_vPuts(szFmt, t_ap);
        return i4Ret;
    }

    i4Len = x_vsnprintf(szStr, LOCAL_BUFSZ-1, szFmt, t_ap);
    ASSERT(i4Len < LOCAL_BUFSZ);

    if (i4ModLvl & _M) {
        iModLen = 2 + (INT32)x_strlen(szMod);
    }
    if (i4ModLvl & _T) {
        iIdLen = x_snprintf(szTId, 12, "[%08x]", _UTIL_Current_Task_Pointer());
    }
    if (i4ModLvl & _X) {
        HAL_GetTime(&rTime);
        iTimeLen = x_snprintf(szTime, 24, "[%d.%06d]", rTime.u4Seconds, rTime.u4Micros);
    }

    /* Check the Len can set into Buf2Mem */
    if ((i4Len + iModLen + iIdLen + iTimeLen + 2) > LOGBUF_FREESZ()) {
        /* XXX what to do? */
        szStr[0] = (CHAR)LOGFULL_MARK;
        i4Ret = _UTIL_Buf2Mem(szStr, 1);
        return i4Ret;
    }

    i4Ret = 0;
    u4State = HalCriticalStart();
    if (i4ModLvl & _M) {
        i4Ret += _UTIL_Buf2Mem("[", 1);
        i4Ret += _UTIL_Buf2Mem(szMod, (INT32)x_strlen(szMod));
        i4Ret += _UTIL_Buf2Mem("]", 1);
    }
    if (i4ModLvl & _T) {
        i4Ret += _UTIL_Buf2Mem(szTId, iIdLen);
    }
    if (i4ModLvl & _X) {
        i4Ret += _UTIL_Buf2Mem(szTime, iTimeLen);
    }
    i4Ret += _UTIL_Buf2Mem(szStr, i4Len);

#ifndef CC_MINI_DRIVER
    if ((i4Level & _S) && (_u4SLogIndex < (MAX_SLOG_NUM-1))) {
        x_memset(_aszSLogBuf+_u4SLogIndex*LOCAL_BUFSZ*sizeof(char), 0, LOCAL_BUFSZ);
        if (i4ModLvl & _M) {
            LINT_SUPPRESS_NEXT_EXPRESSION(534);
            x_strncat(_aszSLogBuf+_u4SLogIndex*LOCAL_BUFSZ*sizeof(char), "[", 1);
            LINT_SUPPRESS_NEXT_EXPRESSION(534);
            x_strncat(_aszSLogBuf+_u4SLogIndex*LOCAL_BUFSZ*sizeof(char), szMod, 8);
            LINT_SUPPRESS_NEXT_EXPRESSION(534);
            x_strncat(_aszSLogBuf+_u4SLogIndex*LOCAL_BUFSZ*sizeof(char), "]", 1);
        }
        if (i4ModLvl & _T) {
            LINT_SUPPRESS_NEXT_EXPRESSION(534);
            x_strncat(_aszSLogBuf+_u4SLogIndex*LOCAL_BUFSZ*sizeof(char), szTId, 12);
        }
        if (i4ModLvl & _X) {
            LINT_SUPPRESS_NEXT_EXPRESSION(534);
            x_strncat(_aszSLogBuf+_u4SLogIndex*LOCAL_BUFSZ*sizeof(char), szTime, 24);
        }
        LINT_SUPPRESS_NEXT_EXPRESSION(534);
        x_strncat(_aszSLogBuf+(_u4SLogIndex++)*LOCAL_BUFSZ*sizeof(char), szStr, 80);
    }
#endif /* CC_MINI_DRIVER */

    HalCriticalEnd(u4State);
    x_thread_resume(_hPThread);
    return i4Ret;
}


LOG_LEVEL_FUNCTIONS(CLI,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(GFX,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(GFXSC,  (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(IMGRZ,  (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(OSD,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(PMX,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(VDP,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(TVE,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(NET,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(TVD,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(MPV,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(AUD,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(SMC,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(IDE,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(DMX,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(PSR,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(UART,   (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(FCI,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(IR,     (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(NAND,   (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(MSDC,   (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(NOR,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(FBM,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(VIN,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(SIF,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(PWM,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(GPIO,	(DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(EEPROM,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(FAT,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(FEEDER, (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(MID,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(PVR,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(PCMCIA, (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(DV,     (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(DVMGR,  (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(NPTV,   (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(MJC, (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(I1394,  (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(MD,     (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(BWT,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(MUSB,   (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(RTC,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(PDWNC,  (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(NAV,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(PSIPSR, (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(BLKMGR, (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(JPG,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(PNG,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(WEBP,   (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(SWDMX,  (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(PLAYMGR,(DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(EXTMJC, (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(GDMA,   (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(FVR,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(VDEC,   (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(VOMX,   (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(VENC,   (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(MUXER,  (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(NIM,    (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(LINUX,  (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(MEMTEST,(DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(MUTE,  (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(SCPOS,  (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(NR,  (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(DI,  (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(OS,  (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(EXTRA,  (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(CEC, (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(CBUS, (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(HDMI, (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(DVI, (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(GCPU, (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(CUST1, (DEFAULT_LEVEL | _M | _L))
LOG_LEVEL_FUNCTIONS(CUST2, (DEFAULT_LEVEL | _M | _L))

//lint -restore

#endif // CC_MTK_LOADER

