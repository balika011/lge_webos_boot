//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
#include "x_ckgen.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_5381.h"
#include "dsp_common.h"
#include "ctrl_para.h"
#include "adsp_task.h"
#include "dsp_uop.h"
#include "dsp_shm.h"
#include "dsp_intf.h"
#include "dsp_func.h"
#include "dsp_rg_ctl.h"
#include "aud_debug.h"
#include "aud_hw.h"
#include "fbm_drvif.h"  // for DSP memory allocation
#include "drv_common.h"
#include "drv_adsp.h"
#include "drvcust_if.h"
#include "aud_if.h"

#include "x_os.h"
#include "x_assert.h"

#include "hw_ckgen.h"

#ifdef LINUX_TURNKEY_SOLUTION
#include <linux/module.h>
#endif

LINT_EXT_HEADER_END

//lint -e950 use __align()

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

#ifdef CC_AUD_ARM_SUPPORT
extern void vA2MCommNotifyKernelLog (void);
extern void vA2MCommKernelOpenDone (void);
extern void vA2MCommPostProcOpenDone (void);
extern void vA2MCommPostProcCloseDone (void);

extern void vA2MCommNotifyKernelDone (void);
extern void vA2MCommDec0OpenDone (void);
extern void vA2MCommDec1OpenDone (void);
extern void vA2MCommDec2OpenDone (void);
extern void vA2MCommDec2ADOpenDone (void);
extern void vA2MCommDec3OpenDone (void);
extern void vA2MCommDec0CloseDone (void);
extern void vA2MCommDec1CloseDone (void);
extern void vA2MCommDec2CloseDone (void);
extern void vA2MCommDec2ADCloseDone (void);
extern void vA2MCommDec3CloseDone (void);
extern void vA2MCommIoInitDone (void);
extern void vA2MCommAprocOpenDone (void);
extern void vA2MSwDec0UpdateRp(void);
extern void vA2MMixSound0UpdateRp(void);
extern void vA2MMixSound1UpdateRp(void);
extern void vA2MMixSound2UpdateRp(void);
extern void vA2MMixSound3UpdateRp(void);
extern void vA2MMixSound4UpdateRp(void);
extern void vA2MMixSound5UpdateRp(void);
extern void vA2MMixSound6UpdateRp(void);
extern void vA2MMixSound7UpdateRp(void);
extern void vA2MMixSound8UpdateRp(void);
extern void vA2MMixSound9UpdateRp(void);
extern void vA2MUploadUpdateWp(void);
extern void vA2MUpload2UpdateWp(void);
extern void vA2MRiscPostUpdateWp(void); //parson riscpost
extern void vA2MRiscPostUpdateRp(void);
extern void vA2MNotifySRC (void);
extern void vA2MNotifyAPTS0(void);
extern void vA2MNotifyAPTS1(void);
extern void vA2MNotifyAPTS2(void);
extern void vA2MNotifyHoldComplete0(void);
extern void vA2MNotifyHoldComplete1(void);
extern void vA2MNotifyHoldComplete2(void);
extern void vA2MNotifyUpdateRountine(void);
extern void vAproc_Open (void);
extern UINT32 u4Aproc_IsEnable(void);
extern BOOL bIsSupportAproc (void);
extern void AUD_AprocSwDecUpdateRp(UINT8 u1ID, UINT32 u4Rp);
extern void AUD_AprocMixSndUpdateRp(UINT8 u1ID, UINT32 u4Rp);
extern void AUD_AprocUploadUpdateWp(UINT32 u4Wp);
extern void AUD_AprocUpload2UpdateWp(UINT32 u4Wp);
extern void AUD_AprocRiscPostUpdateWp(UINT32 u4Wp);  //parson riscpost
extern void AUD_AprocRiscPostUpdateRp(UINT32 u4Rp);
extern void AUD_Aproc_Calc_StcDiff(UINT8 u1DecID, UINT32 u4PTS);
extern void AUD_Aproc_Chg_AVSyncCtrl(UINT8 u1DecID, APROC_AVSYNC_CTRL_MODE_T eMode);


extern UINT32 u4GetAprocMemoryMap (UINT32 u4Type);
extern UINT32 u4GetDspWorkBufNonInit(void);

extern UINT32 u4AUD_DrvGetPtsTableAddr(void);
extern UINT32 u4AUD_DrvGetPtsTableSize(void);

extern void vAprocCmdSet (UINT32 u4Idx, UINT32 u4Data0, UINT32 u4Data1);
void vAprocUploadUpdateWp(UINT32 u4Wp);
void vAprocUpload2UpdateWp(UINT32 u4Wp);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL _fgArmIrqRegister = FALSE;

typedef enum {
    APROC_POSTPROC_CLOSE = 0,
    APROC_POSTPROC_OPEN
} APROC_POSTPROC_STATUS_T;

typedef enum {
    APROC_DEV_CLOSE = 0,
    APROC_DEV_OPEN
} APROC_DEV_FLAG_T;


typedef enum {
    APROC_DEV_DEC0 = 0,
    APROC_DEV_DEC1,
    APROC_DEV_DEC2,
    APROC_DEV_DEC3,
    APROC_DEV_DEC2_AD,
    APROC_DEV_DEC_NUM
} APROC_DEV_DEC_T;


static HANDLE_T _hAprocAccessSema;
static HANDLE_T _hAprocSema;
static HANDLE_T _ahSemaAprocComDone;

static char _gcStringBuf[AUD_APROC_CHAR_LEN] ;

static volatile UINT32 _gu4AprocPostProcControl = APROC_POSTPROC_CLOSE;
static volatile UINT32 _gu4AprocDecControl[APROC_DEV_DEC_NUM] = {APROC_POSTPROC_CLOSE, APROC_POSTPROC_CLOSE, APROC_POSTPROC_CLOSE, APROC_POSTPROC_CLOSE};
static volatile UINT32 _gu4AprocControl = APROC_DEV_CLOSE;

_AUD_APROC_COMM_SIG_T _gu4AudAprocA2MSigInfo[] = {
 {APROC_SIG_IDX_KERNEL, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_DONE, 0, (AUD_APROC_FUNC_T) &vA2MCommNotifyKernelDone},
 {APROC_SIG_IDX_KERNEL, APROC_SIG_CMD_OPEN, APROC_COMMAND_DONE, 0, (AUD_APROC_FUNC_T) &vA2MCommKernelOpenDone},
#ifdef CC_AUD_ARM_POST
 {APROC_SIG_IDX_POSTPROC, APROC_SIG_CMD_OPEN, APROC_COMMAND_DONE, 0, (AUD_APROC_FUNC_T) &vA2MCommPostProcOpenDone},
 {APROC_SIG_IDX_POSTPROC, APROC_SIG_CMD_CLOSE, APROC_COMMAND_DONE, 0, (AUD_APROC_FUNC_T) &vA2MCommPostProcCloseDone},
#endif  
 {APROC_SIG_IDX_DEC0, APROC_SIG_CMD_OPEN, APROC_COMMAND_DONE, 0, (AUD_APROC_FUNC_T) &vA2MCommDec0OpenDone},
 {APROC_SIG_IDX_DEC1, APROC_SIG_CMD_OPEN, APROC_COMMAND_DONE, 0, (AUD_APROC_FUNC_T) &vA2MCommDec1OpenDone},
 {APROC_SIG_IDX_DEC2, APROC_SIG_CMD_OPEN, APROC_COMMAND_DONE, 0, (AUD_APROC_FUNC_T) &vA2MCommDec2OpenDone},
 {APROC_SIG_IDX_DEC2_AD, APROC_SIG_CMD_OPEN, APROC_COMMAND_DONE, 0, (AUD_APROC_FUNC_T) &vA2MCommDec2ADOpenDone},
 {APROC_SIG_IDX_DEC3, APROC_SIG_CMD_OPEN, APROC_COMMAND_DONE, 0, (AUD_APROC_FUNC_T) &vA2MCommDec3OpenDone},
 {APROC_SIG_IDX_DEC0, APROC_SIG_CMD_CLOSE, APROC_COMMAND_DONE, 0, (AUD_APROC_FUNC_T) &vA2MCommDec0CloseDone},
 {APROC_SIG_IDX_DEC1, APROC_SIG_CMD_CLOSE, APROC_COMMAND_DONE, 0, (AUD_APROC_FUNC_T) &vA2MCommDec1CloseDone},
 {APROC_SIG_IDX_DEC2, APROC_SIG_CMD_CLOSE, APROC_COMMAND_DONE, 0, (AUD_APROC_FUNC_T) &vA2MCommDec2CloseDone},
 {APROC_SIG_IDX_DEC2_AD, APROC_SIG_CMD_CLOSE, APROC_COMMAND_DONE, 0, (AUD_APROC_FUNC_T) &vA2MCommDec2ADCloseDone},
 {APROC_SIG_IDX_DEC3, APROC_SIG_CMD_CLOSE, APROC_COMMAND_DONE, 0, (AUD_APROC_FUNC_T) &vA2MCommDec3CloseDone},
 {APROC_SIG_IDX_IOBUF, APROC_SIG_CMD_INIT, APROC_COMMAND_DONE, 0, (AUD_APROC_FUNC_T) &vA2MCommIoInitDone},
 {APROC_SIG_IDX_APROC, APROC_SIG_CMD_OPEN, APROC_COMMAND_DONE, 0, (AUD_APROC_FUNC_T) &vA2MCommAprocOpenDone},
#ifdef CC_AUD_ARM_RENDER 
 {APROC_SIG_IDX_DEC3, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_UPDATE_RP, 0, (AUD_APROC_FUNC_T) &vA2MSwDec0UpdateRp},  
 {APROC_SIG_IDX_MIXSND0, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_UPDATE_RP, 0, (AUD_APROC_FUNC_T) &vA2MMixSound0UpdateRp},  //parson mixsnd
 {APROC_SIG_IDX_MIXSND1, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_UPDATE_RP, 0, (AUD_APROC_FUNC_T) &vA2MMixSound1UpdateRp},
 {APROC_SIG_IDX_MIXSND2, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_UPDATE_RP, 0, (AUD_APROC_FUNC_T) &vA2MMixSound2UpdateRp},  //parson mixsnd
 {APROC_SIG_IDX_MIXSND3, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_UPDATE_RP, 0, (AUD_APROC_FUNC_T) &vA2MMixSound3UpdateRp},
 {APROC_SIG_IDX_MIXSND4, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_UPDATE_RP, 0, (AUD_APROC_FUNC_T) &vA2MMixSound4UpdateRp},  //parson mixsnd
 {APROC_SIG_IDX_MIXSND5, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_UPDATE_RP, 0, (AUD_APROC_FUNC_T) &vA2MMixSound5UpdateRp},
 {APROC_SIG_IDX_MIXSND6, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_UPDATE_RP, 0, (AUD_APROC_FUNC_T) &vA2MMixSound6UpdateRp},  //parson mixsnd
 {APROC_SIG_IDX_MIXSND7, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_UPDATE_RP, 0, (AUD_APROC_FUNC_T) &vA2MMixSound7UpdateRp},
 {APROC_SIG_IDX_HW_MIXSND0, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_UPDATE_RP, 0, (AUD_APROC_FUNC_T) &vA2MMixSound8UpdateRp},
 {APROC_SIG_IDX_HW_MIXSND1, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_UPDATE_RP, 0, (AUD_APROC_FUNC_T) &vA2MMixSound9UpdateRp},
 {APROC_SIG_IDX_UPLOAD0, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_UPDATE_WP, 0, (AUD_APROC_FUNC_T) &vA2MUploadUpdateWp}, 
 {APROC_SIG_IDX_UPLOAD2, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_UPDATE_WP, 0, (AUD_APROC_FUNC_T) &vA2MUpload2UpdateWp},  
 {APROC_SIG_IDX_SRC, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_SRC, 0, (AUD_APROC_FUNC_T) &vA2MNotifySRC},
 {APROC_SIG_IDX_POSTPROC, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_MCPU_UPDATE_ROUNTIE, 0, (AUD_APROC_FUNC_T) &vA2MNotifyUpdateRountine},
 {APROC_SIG_IDX_RISCPOST, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_UPDATE_WP, 0, (AUD_APROC_FUNC_T) &vA2MRiscPostUpdateWp},  //parson riscpost
 {APROC_SIG_IDX_RISCPOST, APROC_SIG_CMD_NOTIFY, APROC_SIG_NOTIFY_UPDATE_RP, 0, (AUD_APROC_FUNC_T) &vA2MRiscPostUpdateRp},  
#endif 
  
 {0, 0, 0, 0, 0}
};


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
/* Semaphore for Audio processor resource accessing
*/
void vAprocLockAccessSema(void)
{
    VERIFY(x_sema_lock(_hAprocAccessSema, X_SEMA_OPTION_WAIT)== OSR_OK);
}

void vAprocUnlockAccessSema(void)
{
    VERIFY(x_sema_unlock(_hAprocAccessSema)== OSR_OK);
}

/* Semaphore for Audio processor
*/
void vAprocLockSema(void)
{
    VERIFY(x_sema_lock(_hAprocSema, X_SEMA_OPTION_WAIT)== OSR_OK);
}

void vAprocUnlockSema(void)
{
    VERIFY(x_sema_unlock(_hAprocSema)== OSR_OK);
}

/* audio processor command done
*/
void vAprocCmdDone (UINT32 u4Command)
{
    LOG (7, "vAprocCmdDone: 0x%x\n", u4Command);
    
    VERIFY(x_sema_unlock(_ahSemaAprocComDone) == OSR_OK);
} 

/* audio processor wait command done
*/
void vAprocWaitCmdDone (void)
{
    INT32 i4Ret;

    LOG(5, "vAprocWaitCmdDone ...\n");

#ifndef FIX_AUD_UOP_TIMEOUT
    // Try lock to make sure the semaphore was locked
    i4Ret = x_sema_lock(_ahSemaAprocComDone, X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
#endif

    // Real lock ..
    i4Ret = x_sema_lock_timeout(_ahSemaAprocComDone, 5000);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));

    LOG(5, "vAprocWaitCmdDone: wake-up!\n");
    
    if (i4Ret == OSR_TIMEOUT)
    {
        LOG(1, "vAprocWaitCmdDone TIMEOUT\n");
        // fix me!!!! ....
    }
}


/* Add [A] to Log
*/
//extern UINT32 u4AprocGetWorkSpaceSize (void);
//extern UINT32 u4GetAprocMemoryMap (UINT32 u4Type);
static CHAR *_cAudAprocLogTransfer (CHAR *cIn )
{
    UINT32 i;

    // provide prefix
    _gcStringBuf[0] = '[';
    _gcStringBuf[1] = 'A';
    _gcStringBuf[2] = ']';
    DSP_FlushInvalidateDCacheFree(u4GetAprocMemoryMap(APROC_MEMMAP_WORKBUF_PHY)+u4AprocBIM_read (APROC_A2M_LOG_ADDR) - 0x20, 0x40);
    // copy stream
    for (i = 0; i < AUD_APROC_CHAR_NUM; i ++)
    {
        _gcStringBuf[i+3] = (*((volatile CHAR*)(&cIn[i])));         
    }   

    // provide end
    _gcStringBuf[AUD_APROC_CHAR_LEN-2] = 0x00;
    _gcStringBuf[AUD_APROC_CHAR_LEN-1] = 0x0A;

    return (CHAR *) _gcStringBuf;
}

/* Audio processor, Audio processor to Main ARM log handler
*/

// ISR handler for Audio processor Logs
static void vAprocCommLog_handler (void)
{
    INT32 i4Val0, i4Val1, i4Val2, i4Val3;
    CHAR *cLog;

    i4Val0 = u4AprocBIM_read (APROC_A2M_LOG_DATA0);
    i4Val1 = u4AprocBIM_read (APROC_A2M_LOG_DATA1);
    i4Val2 = u4AprocBIM_read (APROC_A2M_LOG_DATA2);
    i4Val3 = u4AprocBIM_read (APROC_A2M_LOG_DATA3);
    
    cLog= _cAudAprocLogTransfer ((CHAR *) (u4AprocBIM_read (APROC_A2M_LOG_ADDR) + 
        u4GetAprocMemoryMap(APROC_MEMMAP_WORKBUF)));

    LOG (0, cLog, i4Val0, i4Val1, i4Val2, i4Val3);    
}

// ISR handler for Audio processor ISR Logs
#ifndef  CC_AUD_ARM_RENDER //This Interrupt has been changed to IsrSignal usage in Render case.
static void vAprocCommIsrLog_handler (void)
{
    INT32 i4Val0, i4Val1, i4Val2, i4Val3;
    CHAR *cLog;

    i4Val0 = u4AprocBIM_read (APROC_A2M_ISR_LOG_DATA0);
    i4Val1 = u4AprocBIM_read (APROC_A2M_ISR_LOG_DATA1);
    i4Val2 = u4AprocBIM_read (APROC_A2M_ISR_LOG_DATA2);
    i4Val3 = u4AprocBIM_read (APROC_A2M_ISR_LOG_DATA3);

    cLog = _cAudAprocLogTransfer ((CHAR *) (u4AprocBIM_read (APROC_A2M_ISR_LOG_ADDR) + 
        u4GetAprocMemoryMap(APROC_MEMMAP_WORKBUF)));

    LOG (7, cLog, i4Val0, i4Val1, i4Val2, i4Val3);
}
#endif

volatile UINT32 _gsu4AprocKernelDoneFlag = 0;

void vA2MCommNotifyKernelDone (void)
{
    _gsu4AprocKernelDoneFlag =  u4AprocBIM_read (APROC_A2M_DATA1_REG);
}

void vA2MCommKernelOpenDone (void)
{
    LOG (5, "====> vA2MCommKernelOpenDone\n");
}

void vA2MCommDec0OpenDone (void)
{
    LOG (5, "====> vA2MCommDec0OpenDone\n");
    
    vAprocCmdDone (APROC_SIGNAL (APROC_SIG_IDX_DEC0, APROC_SIG_CMD_OPEN));    
}

void vA2MCommDec1OpenDone (void)
{
    LOG (5, "====> vA2MCommDec1OpenDone\n");
    
    vAprocCmdDone (APROC_SIGNAL (APROC_SIG_IDX_DEC1, APROC_SIG_CMD_OPEN));    
}

void vA2MCommDec2OpenDone (void)
{
    LOG (5, "====> vA2MCommDec2OpenDone\n");
    
    vAprocCmdDone (APROC_SIGNAL (APROC_SIG_IDX_DEC2, APROC_SIG_CMD_OPEN));    
}

void vA2MCommDec2ADOpenDone (void)
{
    LOG (5, "====> vA2MCommDec2ADOpenDone\n");
    
    vAprocCmdDone (APROC_SIGNAL (APROC_SIG_IDX_DEC2_AD, APROC_SIG_CMD_OPEN));    
}

void vA2MCommDec3OpenDone (void)
{
    LOG (5, "====> vA2MCommDec3OpenDone\n");
    
    vAprocCmdDone (APROC_SIGNAL (APROC_SIG_IDX_DEC3, APROC_SIG_CMD_OPEN));    
}

void vA2MCommAprocOpenDone (void)
{
    LOG (5, "====> vA2MCommAprocOpenDone\n");
    
    vAprocCmdDone (APROC_SIGNAL (APROC_SIG_IDX_APROC, APROC_SIG_CMD_OPEN));   
}


void vA2MCommDec0CloseDone (void)
{
    LOG (5, "====> vA2MCommDec0CloseDone\n");
    vAprocCmdDone (APROC_SIGNAL (APROC_SIG_IDX_DEC0, APROC_SIG_CMD_CLOSE));
    _gu4AprocDecControl[APROC_DEV_DEC0] = APROC_DEV_OPEN;
}

void vA2MCommDec1CloseDone (void)
{
    LOG (5, "====> vA2MCommDec1CloseDone\n");
    vAprocCmdDone (APROC_SIGNAL (APROC_SIG_IDX_DEC1, APROC_SIG_CMD_CLOSE));
    _gu4AprocDecControl[APROC_DEV_DEC1] = APROC_DEV_OPEN;
}

void vA2MCommDec2CloseDone (void)
{
    LOG (5, "====> vA2MCommDec2CloseDone\n");
    vAprocCmdDone (APROC_SIGNAL (APROC_SIG_IDX_DEC2, APROC_SIG_CMD_CLOSE));
    _gu4AprocDecControl[APROC_DEV_DEC2] = APROC_DEV_OPEN;
}

void vA2MCommDec2ADCloseDone (void)
{
    LOG (5, "====> vA2MCommDec2ADCloseDone\n");
    vAprocCmdDone (APROC_SIGNAL (APROC_SIG_IDX_DEC2_AD, APROC_SIG_CMD_CLOSE));
    _gu4AprocDecControl[APROC_DEV_DEC2_AD] = APROC_DEV_OPEN;
}

void vA2MCommDec3CloseDone (void)
{
    LOG (5, "====> vA2MCommDec3CloseDone\n");
    vAprocCmdDone (APROC_SIGNAL (APROC_SIG_IDX_DEC3, APROC_SIG_CMD_CLOSE));
    _gu4AprocDecControl[APROC_DEV_DEC3] = APROC_DEV_OPEN;
}

#ifdef CC_AUD_ARM_RENDER
void vA2MSwDec0UpdateRp(void)
{
    UINT32 u4Rp;

    u4Rp = u4AprocBIM_read (APROC_A2M_DATA1_REG);
    LOG (9, "====> vA2MSwDec0UpdateRp  %d\n", u4Rp);    
    AUD_AprocSwDecUpdateRp(0, u4Rp); 
}
void vA2MMixSound0UpdateRp(void)//parson mixsnd
{
    UINT32 u4Rp;

    u4Rp = u4AprocBIM_read (APROC_A2M_DATA1_REG);
    LOG (9, "====> vA2MMixSound0UpdateRp  %d\n", u4Rp);    
    AUD_AprocMixSndUpdateRp(0, u4Rp); //Wait MixSnd OK
}
void vA2MMixSound1UpdateRp(void)
{
    UINT32 u4Rp;

    u4Rp = u4AprocBIM_read (APROC_A2M_DATA1_REG);
    LOG (9, "====> vA2MMixSound1UpdateRp  %d\n", u4Rp);    
    AUD_AprocMixSndUpdateRp(1, u4Rp); //Wait MixSnd OK
}
//#ifdef CC_AUD_APROC_HWMIX
void vA2MMixSound2UpdateRp(void)//parson mixsnd
{
    UINT32 u4Rp;

    u4Rp = u4AprocBIM_read (APROC_A2M_DATA1_REG);
    LOG (9, "====> vA2MMixSound2UpdateRp  %d\n", u4Rp);    
    AUD_AprocMixSndUpdateRp(2, u4Rp); //Wait MixSnd OK
}
void vA2MMixSound3UpdateRp(void)//parson mixsnd
{
    UINT32 u4Rp;

    u4Rp = u4AprocBIM_read (APROC_A2M_DATA1_REG);
    LOG (9, "====> vA2MMixSound3UpdateRp  %d\n", u4Rp);    
    AUD_AprocMixSndUpdateRp(3, u4Rp); //Wait MixSnd OK
}
//#endif
void vA2MMixSound4UpdateRp(void)//parson mixsnd
{
    UINT32 u4Rp;

    u4Rp = u4AprocBIM_read (APROC_A2M_DATA1_REG);
    LOG (9, "====> vA2MMixSound4UpdateRp  %d\n", u4Rp);    
    AUD_AprocMixSndUpdateRp(4, u4Rp); //Wait MixSnd OK
}
void vA2MMixSound5UpdateRp(void)//parson mixsnd
{
    UINT32 u4Rp;

    u4Rp = u4AprocBIM_read (APROC_A2M_DATA1_REG);
    LOG (9, "====> vA2MMixSound5UpdateRp  %d\n", u4Rp);    
    AUD_AprocMixSndUpdateRp(5, u4Rp); //Wait MixSnd OK
}

void vA2MMixSound6UpdateRp(void)//parson mixsnd
{
    UINT32 u4Rp;

    u4Rp = u4AprocBIM_read (APROC_A2M_DATA1_REG);
    LOG (9, "====> vA2MMixSound6UpdateRp  %d\n", u4Rp);    
    AUD_AprocMixSndUpdateRp(6, u4Rp); //Wait MixSnd OK
}

void vA2MMixSound7UpdateRp(void)//parson mixsnd
{
    UINT32 u4Rp;

    u4Rp = u4AprocBIM_read (APROC_A2M_DATA1_REG);
    LOG (9, "====> vA2MMixSound7UpdateRp  %d\n", u4Rp);    
    AUD_AprocMixSndUpdateRp(7, u4Rp); //Wait MixSnd OK
}

void vA2MMixSound8UpdateRp(void)//parson mixsnd
{
    UINT32 u4Rp;

    u4Rp = u4AprocBIM_read (APROC_A2M_DATA1_REG);
    LOG (9, "====> vA2MMixSound8UpdateRp  %d\n", u4Rp);    
    AUD_AprocMixSndUpdateRp(8, u4Rp); //Wait MixSnd OK
}
void vA2MMixSound9UpdateRp(void)//parson mixsnd
{
    UINT32 u4Rp;

    u4Rp = u4AprocBIM_read (APROC_A2M_DATA1_REG);
    LOG (9, "====> vA2MMixSound9UpdateRp  %d\n", u4Rp);    
    AUD_AprocMixSndUpdateRp(9, u4Rp); //Wait MixSnd OK
}

void vA2MUploadUpdateWp(void)
{
    UINT32 u4Wp;

    u4Wp = u4AprocBIM_read (APROC_A2M_DATA1_REG);
    LOG (9, "====> vA2MUploadUpdateWp  %d\n", u4Wp);    
    AUD_AprocUploadUpdateWp(u4Wp); //Wait Upload OK
    vAprocUploadUpdateWp(u4Wp);
}

void vA2MUpload2UpdateWp(void)
{
    UINT32 u4Wp;

    u4Wp = u4AprocBIM_read (APROC_A2M_DATA1_REG);
    LOG (9, "====> vA2MUpload2UpdateWp  %d\n", u4Wp);    
    AUD_AprocUpload2UpdateWp(u4Wp); //Wait Upload OK
    vAprocUpload2UpdateWp(u4Wp);
}
void vA2MRiscPostUpdateWp(void) //parson riscpost
{
    UINT32 u4Wp;

    u4Wp = u4AprocBIM_read (APROC_A2M_DATA1_REG);
    LOG (9, "====> vA2MURiscPostUpdateWp  %d\n", u4Wp);    
    AUD_AprocRiscPostUpdateWp(u4Wp);
}
void vA2MRiscPostUpdateRp(void)
{
    UINT32 u4Rp;

    u4Rp = u4AprocBIM_read (APROC_A2M_DATA1_REG);
    LOG (9, "====> vA2MURiscPostUpdateRp  %d\n", u4Rp);    
    AUD_AprocRiscPostUpdateRp(u4Rp);
}
void vA2MNotifySRC (void)
{
    vA2MNotifyAPTS0 ();
    vA2MNotifyAPTS1 ();
    vA2MNotifyAPTS2 ();
    vA2MNotifyHoldComplete0 ();
    vA2MNotifyHoldComplete1 ();
    vA2MNotifyHoldComplete2 ();
}

void vA2MNotifyUpdateRountine (void)
{
    UINT8 u1DecId;
    UINT32 u4Offset;
    UINT32 pu4DownmixTable[10];
    UINT32 u4DownmixTbl;

    u1DecId = (UINT8)u4AprocBIM_read (APROC_A2M_DATA1_REG);
    if (u1DecId > AUD_DEC_THIRD)
    {
        return;
    }    

    switch (u1DecId)
    {
    case AUD_DEC_MAIN:
        u4Offset = APROC_REG_DMIX_AMIXER0_WAIT;
        u4DownmixTbl = APROC_DATAPORT_DOWNMIX_TBL_BASE;
        break;
    case AUD_DEC_AUX:
        u4Offset = APROC_REG_DMIX_AMIXER1_WAIT;
        u4DownmixTbl = APROC_DATAPORT_DOWNMIX_TBL_1_BASE;        
        break;        
    case AUD_DEC_THIRD:
        u4Offset = APROC_REG_DMIX_AMIXER2_WAIT;
        u4DownmixTbl = APROC_DATAPORT_DOWNMIX_TBL_BASE;        
        break;        
    default:
        return;
    }        
    
    // SetUpDownmix Tables.
    pu4DownmixTable[0] = 0x2555;
    pu4DownmixTable[1] = 0x34CC;
    pu4DownmixTable[2] = 0x0;
    pu4DownmixTable[3] = 0x12AA;
    pu4DownmixTable[4] = 0x0;

    pu4DownmixTable[5] = 0x2555;
    pu4DownmixTable[6] = 0;
    pu4DownmixTable[7] = 0x34CC;
    pu4DownmixTable[8] = 0x0;
    pu4DownmixTable[9] = 0x12AA;
        
    LOG(7, "Dec(%d) : CR - Ch_Num from APROC is %d\n", u1DecId,
            u4AprocReg_Read(APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_4, (APROC_REG_DMIX_AMIXER0_INFO+u1DecId))) );
    vAprocDataPort_Write (u4DownmixTbl, pu4DownmixTable, APROC_DATAPORT_DOWNMIX_TBL_SIZE);

    // Set Done to APROC by ASM
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_4, u4Offset), 0);    
}

void vA2M_AptsIsr(UINT8 u1Id)
{
    UINT32 u4PTS;
    UINT32 u4AsmNfyAddr;
    UINT32 u4AsmPtsAddr;

    switch (u1Id)
    {
    case AUD_DEC_MAIN:
        u4AsmNfyAddr = APROC_REG_AVSYNC_NOTIFY_APTS_SRC0;
        u4AsmPtsAddr = APROC_REG_APTS_SRC0;
        break;        
    case AUD_DEC_AUX:
        u4AsmNfyAddr = APROC_REG_AVSYNC_NOTIFY_APTS_SRC1;        
        u4AsmPtsAddr = APROC_REG_APTS_SRC1;        
        break;        
    case AUD_DEC_THIRD:
        u4AsmNfyAddr = APROC_REG_AVSYNC_NOTIFY_APTS_SRC2;        
        u4AsmPtsAddr = APROC_REG_APTS_SRC2;        
        break;        
    default:
        return;
    }
    
    if (u4AprocReg_Read (APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0, u4AsmNfyAddr)))
    {
        u4PTS = u4AprocReg_Read (APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0, u4AsmPtsAddr));
        AUD_Aproc_Calc_StcDiff(u1Id, u4PTS);
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0, u4AsmNfyAddr), 0x0);
    }
    
}

void vA2MNotifyAPTS0(void)
{
    vA2M_AptsIsr(AUD_DEC_MAIN);
}
void vA2MNotifyAPTS1(void)
{
    vA2M_AptsIsr(AUD_DEC_AUX);
}
void vA2MNotifyAPTS2(void)
{
    vA2M_AptsIsr(AUD_DEC_THIRD);
}

void vA2M_HoldCompleteIsr(UINT8 u1Id)
{
    UINT32 u4PTS;
    UINT32 u4AsmNfyAddr;
    UINT32 u4AsmPtsAddr;

    switch (u1Id)
    {
    case AUD_DEC_MAIN:
        u4AsmNfyAddr = APROC_REG_AVSYNC_NOTIFY_HOLD_OK_SRC0;
        u4AsmPtsAddr = APROC_REG_APTS_SRC0;
        break;
    case AUD_DEC_AUX:
        u4AsmNfyAddr = APROC_REG_AVSYNC_NOTIFY_HOLD_OK_SRC1;        
        u4AsmPtsAddr = APROC_REG_APTS_SRC1;        
        break;        
    case AUD_DEC_THIRD:
        u4AsmNfyAddr = APROC_REG_AVSYNC_NOTIFY_HOLD_OK_SRC2;        
        u4AsmPtsAddr = APROC_REG_APTS_SRC2;        
        break;        
    default:
        return;
    }

    u4PTS = u4AprocReg_Read (APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0, u4AsmPtsAddr));
    if (u4AprocReg_Read (APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0, u4AsmNfyAddr)))
    {
        AUD_Aproc_Chg_AVSyncCtrl(u1Id, APROC_AVSYNC_CTRL_HOLD_COMPLETE);
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0, u4AsmNfyAddr), 0x0);

        LOG (5, "(Aproc_Sync)Aproc HoldComplete(%d), 0x%x\n", u1Id, u4PTS);
        //AUD_DrvMMAoutEnableNotify(u1Id, FALSE);        
    }    
}
void vA2MNotifyHoldComplete0(void)
{
    vA2M_HoldCompleteIsr(AUD_DEC_MAIN);
}
void vA2MNotifyHoldComplete1(void)
{
    vA2M_HoldCompleteIsr(AUD_DEC_AUX);
}
void vA2MNotifyHoldComplete2(void)
{
    vA2M_HoldCompleteIsr(AUD_DEC_THIRD);
}

#endif //CC_AUD_ARM_RENDER

void vA2MCommIoInitDone (void)
{
    LOG (5, "====> vA2MCommIoInitDone\n");
}

void vA2MCommNotifyKernelLog (void)
{
    CHAR *cLog;

    cLog = _cAudAprocLogTransfer ((CHAR *) (u4GetAprocMemoryMap(APROC_MEMMAP_WORKBUF) + 
        u4AprocBIM_read (APROC_A2M_DATA1_REG)));

    LOG (5, cLog);
}

void vA2MCommPostProcOpenDone (void)
{
    LOG (5, "====> vA2MCommPostProcOpenDone\n");
    
    vAprocCmdDone (APROC_SIGNAL (APROC_SIG_IDX_POSTPROC, APROC_SIG_CMD_OPEN));
    _gu4AprocPostProcControl = APROC_POSTPROC_OPEN;
}

void vA2MCommPostProcCloseDone (void)
{
    LOG (5, "====> vA2MCommPostProcCloseDone\n");
    vAprocCmdDone (APROC_SIGNAL (APROC_SIG_IDX_POSTPROC, APROC_SIG_CMD_CLOSE));
    _gu4AprocPostProcControl = APROC_POSTPROC_CLOSE;
}


/* Audio processpr ISR functions
*/ 
static void vAprocCommSignal_handler (void)
{
    UINT32 u4Cmd, u4Idx;
    _AUD_APROC_COMM_SIG_T *eInfo = (_AUD_APROC_COMM_SIG_T *) _gu4AudAprocA2MSigInfo;
    
    u4Cmd = u4AprocBIM_read (APROC_A2M_SIG_REG);
    u4Idx = u4AudAprocSig_GetIdx(u4Cmd);
    u4Cmd = u4AudAprocSig_GetCMD(u4Cmd);

    while (1)
    {
        if ((eInfo->u4Cmd == 0) && (eInfo->u4Idx == 0))
        {
            // exit case
            break;
        }

        if ((eInfo->u4Cmd == u4Cmd) && (eInfo->u4Idx == u4Idx))
        {
            //  command hit
            if (Aproc_Sig_A2M_Is_Hit (eInfo->u4Data0, u4AprocBIM_read (APROC_A2M_DATA0_REG)) &&
                Aproc_Sig_A2M_Is_Hit (eInfo->u4Data1, u4AprocBIM_read (APROC_A2M_DATA1_REG)) )
            {
                if (eInfo->u4Func)
                {
                    eInfo->u4Func ();
                }
                break;
            }
        }
        eInfo ++;
    }
}

#ifdef CC_AUD_ARM_RENDER
static void vAprocISRCommSignal_handler (void)
{
    UINT32 u4Cmd, u4Idx;
    _AUD_APROC_COMM_SIG_T *eInfo = (_AUD_APROC_COMM_SIG_T *) _gu4AudAprocA2MSigInfo;
    
    u4Cmd = u4AprocBIM_read (APROC_A2M_ISR_LOG_ADDR);
    u4Idx = u4AudAprocSig_GetIdx(u4Cmd);
    u4Cmd = u4AudAprocSig_GetCMD(u4Cmd);

    while (1)
    {
        if ((eInfo->u4Cmd == 0) && (eInfo->u4Idx == 0))
        {
            // exit case
            break;
        }

        if ((eInfo->u4Cmd == u4Cmd) && (eInfo->u4Idx == u4Idx))
        {
            //  command hit
            if (Aproc_Sig_A2M_Is_Hit (eInfo->u4Data0, u4AprocBIM_read (APROC_A2M_ISR_LOG_DATA0)) &&
                Aproc_Sig_A2M_Is_Hit (eInfo->u4Data1, u4AprocBIM_read (APROC_A2M_ISR_LOG_DATA1)) )
            {
                if (eInfo->u4Func)
                {
                    eInfo->u4Func ();
                }
                break;
            }
        }
        eInfo ++;
    }
}
#endif //CC_AUD_ARM_RENDER

/* Audio processpr ISR handlers
*/ 
static void vAprocIRQHandler0_ISR(UINT16 u2Vector)
{
    vAprocCommSignal_handler ();
    
    UNUSED(u2Vector);
}

static void vAprocIRQHandler1_ISR(UINT16 u2Vector)
{
    vAprocCommLog_handler ();
   
    UNUSED(u2Vector);
}

static void vAprocIRQHandler2_ISR(UINT16 u2Vector)
{
#ifndef CC_AUD_ARM_RENDER
    vAprocCommIsrLog_handler ();
#else
    vAprocISRCommSignal_handler ();
#endif
     
     UNUSED(u2Vector);
}

static void vAprocIRQHandler3_ISR(UINT16 u2Vector)
{
    LOG (0, "=====> vAprocIRQHandler3_ISR\n");
    
     //....
     
     UNUSED(u2Vector);
}

#if defined (CC_AUD_ARM_RENDER) || defined (CC_AUD_ARM_POST)

//-----------------------------------------------------------------------------
/** vAprocSysClockSelection
 *  APROC Clk cource selection
 *
 *  @param  u4Sel     clock source selection

    0 xtal_d1_ck
    1 tapll_d2_ck
    2 sawlesspll_d2_ck
    3 usb_phy_pll_d1_ck
    4 sawlesspll_d1_ck
    5 cpupll_d2p5_ck
    6 syspll_d2_ck
    7 usb_phy_pll_d2_ck
 *  @retval void
 */
//-----------------------------------------------------------------------------
void vAprocSysClockSelection (UINT32 u4Sel)
{
#ifdef CC_MT5882 
    vIO32WriteFldAlign (CKGEN_REG_ACPU_CKCFG, u4Sel, FLD_ACPU_SEL);
#endif
}
#endif //CC_AUD_ARM_RENDER

/* init audio processor
*/
static void vAproc_Init (void)
{
    UINT32 u4AprocWokBuf;

    //enable Audio ARM clock
    vAprocBIM_write  (APROC_RST, u4AprocBIM_read(APROC_RST) | APROC_CLK_EN);

    // disable Audio ARM
    vAprocBIM_write  (APROC_RST, u4AprocBIM_read(APROC_RST) & ~APROC_RESET);
    
#ifdef CC_AUD_ARM11_SUPPORT
    // reset ABIM -> reset, 1: Normal
    vIO32WriteFldAlign (CKGEN_BLOCK_RST_CFG1, 0, FLD_ABIM_RST);
    vIO32WriteFldAlign (CKGEN_BLOCK_RST_CFG1, 1, FLD_ABIM_RST);

    // reset Audio ARM -> reset, 1: Normal
    vIO32WriteFldAlign (CKGEN_BLOCK_RST_CFG1, 0, FLD_ARM11_RST);
    vIO32WriteFldAlign (CKGEN_BLOCK_RST_CFG1, 1, FLD_ARM11_RST);

    //enable ARM11/ABIM
    vIO32WriteFldAlign (CKGEN_BLOCK_CKEN_CFG1, 1, FLD_ARM11_CKEN);
    vIO32WriteFldAlign (CKGEN_BLOCK_CKEN_CFG1, 1, FLD_ABIM_CKEN);

    //Oryx ES3/Gazllen clock selection
    vAprocBIM_write(APROC_BASE, u4AprocBIM_read(APROC_BASE) | APROC_CLK_SEL);
    
#ifdef CC_MT5882

    // reset Audio ARM
    vIO32WriteFldAlign (CKGEN_REG_ACPU_CKCFG, 1, FLD_ACPU_PD);
    vIO32WriteFldAlign (CKGEN_REG_ACPU_CKCFG, 0, FLD_ACPU_PD);

    // set Audio ARM AHB speed
    vIO32WriteFldAlign (CKGEN_REG_ACPU_CKCFG, 1, FLD_ACPU_MISC); // /2

    // set Audio ARM speed
#if defined (CC_AUD_ARM_RENDER) || defined (CC_AUD_ARM_POST)
    vAprocSysClockSelection (1); // 432 MHz
#else
    vIO32WriteFldAlign (CKGEN_REG_ACPU_CKCFG, 4, FLD_ACPU_SEL); // 432 MHz
#endif
#endif

#endif
    // Initial Audio ARM memory    
    vAprocMemInit ((UINT8 *) u4GetDspWorkBufNonInit());

    // setup base address
    u4AprocWokBuf = u4GetAprocMemoryMap(APROC_MEMMAP_WORKBUF_PHY);
    vAprocBIM_write (APROC_BASE, (u4AprocBIM_read(APROC_BASE) & ~APROC_BASE_MASK) | u4AprocWokBuf);
    
    // load code    
    u4AudAprocLoadCode (u4AprocWokBuf);        
}

/* start audio processor
*/
static void vAproc_start (void)
{
    vAprocBIM_write  (APROC_RST, u4AprocBIM_read(APROC_RST) | APROC_RESET);
}

/* start audio processor
*/
static void vAproc_stop (void)
{
    vAprocBIM_write  (APROC_RST, u4AprocBIM_read(APROC_RST) & ~APROC_RESET);
}


 /******************************************************************************
 * Function      : vAprocMain_Init
 * Description   : initialization Main routine for Audio processor
 * Parameter     : None
 * Return        : None
 ******************************************************************************/
// very important flag!!!
// It controls audio processor driver part enable.
// If audio processor is disable, all functions in driver related to audio proecssor must be "mute".
UINT32 _gu4AprocEnable = 0; // fix me!!!!

void vAproc_Enable(UINT32 u4Enable)
{
    if (u4Enable)
    {
        vAproc_start ();
        // enable ADSP part routine
        vWriteDspWORD (AUD_DSP0, ADDR_RC2D_APROC_CONTROL, 0x0); // enable post
        _gu4AprocEnable = 1;
    }
    else
    {
        vAproc_stop ();
        vWriteDspWORD (AUD_DSP0, ADDR_RC2D_APROC_CONTROL, 0x8000); // bypass post
        _gu4AprocEnable = 0;
    }
}

#ifdef CC_AUD_ARM_POST
void vAprocS_EnableConfig(BOOL fgEnable)
{
    if (fgEnable)
    {
        //vAproc_Enable ();
        // enable ADSP part routine
        vWriteDspWORD (AUD_DSP0, ADDR_RC2D_APROC_CONTROL, 0x0); // enable post
        _gu4AprocEnable = 1;
    }
    else
    {
        //vAproc_Disable ();
        vWriteDspWORD (AUD_DSP0, ADDR_RC2D_APROC_CONTROL, 0x8000); // bypass post
        _gu4AprocEnable = 0;
    }
}
#endif

UINT32 u4Aproc_IsEnable(void)
{
    return _gu4AprocEnable;
}
 
BOOL bIsSupportAproc (void)
{
#if (defined(CC_AUD_ARM_RENDER) || defined(CC_AUD_ARM_POST))
    return 1;//DRVCUST_OptGet(eAudioAprocEnable);
#else
    return 0;
#endif
}
extern void vDspDramAgentSet(void); 
 void vAprocMain_Init (void)
{

    int i;

    vDspDramAgentSet(); //Set Dram agent related before ARM power on

    // init Audio processor
    vAproc_Init ();

    if (! _fgArmIrqRegister)
    {
        x_os_isr_fct pfnOldIsr; 
    // register ISR handler
#if defined(CC_AUD_ARM11_SUPPORT)
        VERIFY(x_reg_isr(VECTOR_ABIM_0, vAprocIRQHandler0_ISR, &pfnOldIsr) == OSR_OK);
        VERIFY(x_reg_isr(VECTOR_ABIM_1, vAprocIRQHandler1_ISR, &pfnOldIsr) == OSR_OK);
        VERIFY(x_reg_isr(VECTOR_ABIM_2, vAprocIRQHandler2_ISR, &pfnOldIsr) == OSR_OK);
        VERIFY(x_reg_isr(VECTOR_ABIM_3, vAprocIRQHandler3_ISR, &pfnOldIsr) == OSR_OK);
#else
        VERIFY(x_reg_isr(VECTOR_ARM9_0, vAprocIRQHandler0_ISR, &pfnOldIsr) == OSR_OK);
        VERIFY(x_reg_isr(VECTOR_ARM9_1, vAprocIRQHandler1_ISR, &pfnOldIsr) == OSR_OK);
        VERIFY(x_reg_isr(VECTOR_ARM9_2, vAprocIRQHandler2_ISR, &pfnOldIsr) == OSR_OK);
        VERIFY(x_reg_isr(VECTOR_ARM9_3, vAprocIRQHandler3_ISR, &pfnOldIsr) == OSR_OK);
#endif // CC_AUD_ARM11_SUPPORT
        _fgArmIrqRegister = TRUE;
    }

    // init sema
    VERIFY(x_sema_create(&_ahSemaAprocComDone, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
    VERIFY(x_sema_create(&_hAprocAccessSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
    VERIFY(x_sema_create(&_hAprocSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);

    // init registers
    for (i = 0; i < APROC_REGMAP_SIZE; i ++)
    {
        vAprocReg_Write (i, 0x0);
    }
    
#ifdef CC_AUD_ARM_RENDER
    // setup APROC register
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_COMM_0, APROC_REG_CPUSIDE_BASE), u4GetAprocMemoryMap(APROC_MEMMAP_WORKBUF_PHY));

    // Some AVSync infomation address
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AVSYNC_0, APROC_REG_PTS_QRY_RESULT_ADDR), 
                                   ((_u4DspBuf[AUD_DSP0] - u4GetAprocMemoryMap(APROC_MEMMAP_WORKBUF_PHY)) + ADDR_DRAM_APROC_QRY_TBL*4) );
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AVSYNC_0, APROC_REG_PTS_TABLE_ADDR), 
                                    u4AUD_DrvGetPtsTableAddr() - u4GetAprocMemoryMap(APROC_MEMMAP_WORKBUF_PHY));
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AVSYNC_0, APROC_REG_PTS_TABLE_SIZE), 
                                    u4AUD_DrvGetPtsTableSize());
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AVSYNC_0, APROC_REG_AFIFO0_ADDR), 
                                    DSP_INTERNAL_ADDR(_u4AFIFO[AUD_DSP0][AUD_DEC_MAIN][0]));
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AVSYNC_0, APROC_REG_AFIFO1_ADDR), 
                                    DSP_INTERNAL_ADDR(_u4AFIFO[AUD_DSP0][AUD_DEC_AUX][0]));
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AVSYNC_0, APROC_REG_AFIFO2_ADDR), 
                                    DSP_INTERNAL_ADDR(_u4AFIFO[AUD_DSP0][AUD_DEC_THIRD][0]));    
    // ASRC HALF PLL mode setting                                   
    if (IS_IC_5890_ES1())
    {
        vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_AENV_0, APROC_REG_AENV_SRC_HALF_PLL_MODE), 0);
    }    
    else
    {
        vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_AENV_0, APROC_REG_AENV_SRC_HALF_PLL_MODE), 1);    
    }
#endif

    // start Audio processor
    if (bIsSupportAproc () == TRUE)
    {
#if defined(CC_AUD_ARM11_SUPPORT)
        //AUD_WRITE32(0x1000, 0xF000F000); // setup DRAM agent for ARM and DSPA/DSPB
                                                 //6000[29] : agent 15 enable (Audio ARM11 and DSPA)
                                                 //6000[28] : agent 14_1 enable (DSPB)
#endif // CC_AUD_ARM11_SUPPORT    
        vAproc_start ();

        // enable ADSP part routine
        vWriteDspWORD (AUD_DSP0, ADDR_RC2D_APROC_CONTROL, 0x0); // enable post
        _gu4AprocEnable = 1;        

#ifdef CC_AUD_ARM_RENDER
        // wait for APROC kernel OK
        while (1)
        {
            if (_gsu4AprocKernelDoneFlag)
            {
                Printf ("====> APROC version = 0x%x\n", _gsu4AprocKernelDoneFlag);
                break;
            }
        }
#endif
    }
    else
    {
        vWriteDspWORD (AUD_DSP0, ADDR_RC2D_APROC_CONTROL, 0x8000); // bypass post
        _gu4AprocEnable = 0;
    }
#ifdef CC_AUD_ARM_RENDER
    DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Set_Init, 0,0, 0,0); 
#endif
}

#ifdef CC_AUD_ARM_RENDER
// if ADSP is not stopped, use this to reopen APROC
void vAprocMain_Open (void)
{
    vAprocMain_Init();

    // open aproc output
    // aproc output registers are switched by adsp kernel code
    // aproc output must be opened after adsp kernel ready
    if (bIsSupportAproc () == TRUE)
    {
        while (1)
        {
            // wait aproc power-on
            if (u4Aproc_IsEnable())
            {
                break;
            }
            x_thread_delay (1);
        }
    }
    vAproc_Open ();
}

// if ADSP is not stopped, use this to close APROC
void vAprocMain_Close (void)
{
    _gu4AprocControl = APROC_DEV_CLOSE;
    _gu4AprocEnable = 0;
    vAproc_stop ();

    VERIFY(x_sema_delete(_ahSemaAprocComDone) == OSR_OK);
    VERIFY(x_sema_delete(_hAprocAccessSema) == OSR_OK);
    VERIFY(x_sema_delete(_hAprocSema) == OSR_OK);      
}

//-----------------------------------------------------------------------------
/** vAproc_Open
 *  Audio processor open 
 *
 *  @param           
 *  @retval 
 */
//-----------------------------------------------------------------------------
void vAproc_Open (void)
{
    if (!_gu4AprocEnable)
    {
        return;
    }

    if (_gu4AprocControl == APROC_DEV_OPEN)
    {
        LOG (5, "vAproc_Open: already opened!\n");
        return;
    }
    
    LOG (5, "====> vAproc_Open\n");
    vAprocCmdOpen (APROC_SIG_IDX_APROC);
    vAprocWaitCmdDone ();

    _gu4AprocControl = APROC_DEV_OPEN;
}

//-----------------------------------------------------------------------------
/** vAproc_Close
 *  Audio processor close 
 *
 *  @param           
 *  @retval 
 */
//-----------------------------------------------------------------------------
void vAproc_Close (void)
{
    if (!_gu4AprocEnable)
    {
        return;
    }

    if (_gu4AprocControl == APROC_DEV_CLOSE)
    {
        LOG (5, "vAproc_Close: already closed!\n");
        return;
    }

    LOG (5, "====> vAproc_Close\n");
    vAprocCmdClose (APROC_SIG_IDX_APROC);
    vAprocWaitCmdDone ();
    
    _gu4AprocControl = APROC_DEV_CLOSE; 
}

//-----------------------------------------------------------------------------
/** vAprocPostProc_Open
 *  Audio processor open decoders
 *
 *  @param           
 *  @retval 
 */
//-----------------------------------------------------------------------------
void vAprocDec_Open (UINT32 u4MixerId)
{
    UINT32 u4Idx;

    if (!_gu4AprocEnable)
    {
        return;
    }

    if (_gu4AprocDecControl[u4MixerId] == APROC_DEV_OPEN)
    {
        return;
    }

    LOG (5, "====> vAprocDecc_Open %d\n", u4MixerId);

    // init IO
    switch (u4MixerId)
    {
    case APROC_DEV_DEC0:
        u4Idx = APROC_SIG_IDX_DEC0;    
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC0_START), 
            (u4GetAprocMemoryMap(APROC_MEMMAP_ADSP_CMPT) + APROC_ADSP_IOBUF_DEC0_START));
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC0_END), APROC_ADSP_IOBUF_DEC0_END);    
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC0_SIZE), APROC_ADSP_IOBUF_DEC0_SIZE);   
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC0_CH), APROC_ADSP_IOBUF_DEC0_CH);   
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC0_SI_BASE), 
            (u4GetAprocMemoryMap(APROC_MEMMAP_ADSP_DRAM) + APROC_ADSP_IOBUF_DEC0_SI_BASE));

        // paul record
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_ENC_MP3, APROC_REG_IOBUF_INTBUF4_START),
            (u4GetAprocMemoryMap(APROC_MEMMAP_ADSP_CMPT) + APROC_ADSP_IOBUF_INTBUF4_START)); 
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_ENC_MP3, APROC_REG_IOBUF_INTBUF4_END), APROC_ADSP_IOBUF_INTBUF4_END);    
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_ENC_MP3, APROC_REG_IOBUF_INTBUF4_SIZE), APROC_ADSP_IOBUF_INTBUF4_SIZE);   
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_ENC_MP3, APROC_REG_IOBUF_INTBUF4_CH), APROC_ADSP_IOBUF_INTBUF4_CH);
        break;    
    case APROC_DEV_DEC1:
        u4Idx = APROC_SIG_IDX_DEC1;       
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC1_START), 
            (u4GetAprocMemoryMap(APROC_MEMMAP_ADSP_CMPT) + APROC_ADSP_IOBUF_DEC1_START));
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC1_END), APROC_ADSP_IOBUF_DEC1_END);    
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC1_SIZE), APROC_ADSP_IOBUF_DEC1_SIZE);   
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC1_CH), APROC_ADSP_IOBUF_DEC1_CH);      
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC1_SI_BASE), 
            (u4GetAprocMemoryMap(APROC_MEMMAP_ADSP_DRAM) + APROC_ADSP_IOBUF_DEC1_SI_BASE));
        
        // paul record
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_ENC_MP3, APROC_REG_IOBUF_INTBUF4_START),
            (u4GetAprocMemoryMap(APROC_MEMMAP_ADSP_CMPT) + APROC_ADSP_IOBUF_INTBUF4_START)); 
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_ENC_MP3, APROC_REG_IOBUF_INTBUF4_END), APROC_ADSP_IOBUF_INTBUF4_END);    
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_ENC_MP3, APROC_REG_IOBUF_INTBUF4_SIZE), APROC_ADSP_IOBUF_INTBUF4_SIZE);   
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_ENC_MP3, APROC_REG_IOBUF_INTBUF4_CH), APROC_ADSP_IOBUF_INTBUF4_CH);
        break;
    case APROC_DEV_DEC2: 
        u4Idx = APROC_SIG_IDX_DEC2;
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC2_START), 
            (u4GetAprocMemoryMap(APROC_MEMMAP_ADSP_CMPT) + APROC_ADSP_IOBUF_DEC2_START));
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC2_END), APROC_ADSP_IOBUF_DEC2_END);    
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC2_SIZE), APROC_ADSP_IOBUF_DEC2_SIZE);   
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC2_CH), APROC_ADSP_IOBUF_DEC2_CH);      
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC2_SI_BASE), 
            (u4GetAprocMemoryMap(APROC_MEMMAP_ADSP_DRAM) + APROC_ADSP_IOBUF_DEC2_SI_BASE));
        break;
	case APROC_DEV_DEC2_AD: 
        u4Idx = APROC_SIG_IDX_DEC2_AD;
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC2_START), 
            (u4GetAprocMemoryMap(APROC_MEMMAP_ADSP_CMPT) + APROC_ADSP_IOBUF_DEC2_START));
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC2_END), APROC_ADSP_IOBUF_DEC2_END);    
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC2_SIZE), APROC_ADSP_IOBUF_DEC2_SIZE);   
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC2_CH), APROC_ADSP_IOBUF_DEC2_CH);      
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_DEC2_SI_BASE), 
            (u4GetAprocMemoryMap(APROC_MEMMAP_ADSP_DRAM) + APROC_ADSP_IOBUF_DEC2_SI_BASE));
        break;
    case APROC_DEV_DEC3: 
        u4Idx = APROC_SIG_IDX_DEC3;        
        break;           
        
    default:
        LOG (5, "vAprocDecc_Open: wrong decoder ID = %d!\n", u4MixerId);
        return;
    }
    
    vAprocCmdOpen (u4Idx);
    vAprocWaitCmdDone ();

    _gu4AprocDecControl[u4MixerId] = APROC_DEV_OPEN;
}

//-----------------------------------------------------------------------------
/** vAprocDec_Close
 *  Audio processor close decoders
 *
 *  @param           
 *  @retval 
 */
//-----------------------------------------------------------------------------
void vAprocDec_Close (UINT32 u4MixerId)
{
    UINT32 u4Idx;

    if (!_gu4AprocEnable)
    {
        return;
    }

    if (_gu4AprocDecControl[u4MixerId] == APROC_DEV_CLOSE)
    {
        return;
    }

    LOG (5, "====> vAprocDec_Close %d\n", u4MixerId);
    
    switch (u4MixerId)
    {
    case APROC_DEV_DEC0:
        u4Idx = APROC_SIG_IDX_DEC0;
        break;
    case APROC_DEV_DEC1:
        u4Idx = APROC_SIG_IDX_DEC1;
        break;
    case APROC_DEV_DEC2:
        u4Idx = APROC_SIG_IDX_DEC2;
        break;
	case APROC_DEV_DEC2_AD:
		u4Idx = APROC_SIG_IDX_DEC2_AD;
		break;
 	case APROC_DEV_DEC3: 
        u4Idx = APROC_SIG_IDX_DEC3;     //Enhanced Amixer   
        break;        
    default:
        LOG (5, "vAprocDec_Close: wrong decoder ID = %d!\n", u4MixerId);
        return;  
    }
    
    vAprocCmdClose (u4Idx);
    vAprocWaitCmdDone ();

    _gu4AprocDecControl[u4MixerId] = APROC_DEV_CLOSE;
}
#endif  //CC_AUD_ARM_RENDER


//-----------------------------------------------------------------------------
/** vAprocPostProc_Open
 *  Audio processor open post-processing
 *
 *  @param           
 *  @retval 
 */
//-----------------------------------------------------------------------------
void vAprocPostProc_Open (void)
{
#ifdef CC_AUD_ARM_RENDER
    return;
#endif

    LOG (5, "====> vAprocPostProc_Open\n");

    if (!_gu4AprocEnable)
    {
        return;
    }

    if (_gu4AprocPostProcControl == APROC_POSTPROC_OPEN)
    {
        return;
    }

    // init IO
    vAprocReg_Write (APROC_REG_IOBUF_IN_START, 
        u4GetAprocMemoryMap(APROC_MEMMAP_ADSP_CMPT)+(4*ADDR_CMPT_APROC_IO_INBUF_ADDR));
    vAprocReg_Write (APROC_REG_IOBUF_IN_END, 32);
    vAprocReg_Write (APROC_REG_IOBUF_IN_SIZE, 2 * 256 * 4);
    vAprocReg_Write (APROC_REG_IOBUF_IN_SI_BASE, 
        u4GetAprocMemoryMap(APROC_MEMMAP_ADSP_DRAM)+(4*ADDR_APROC_IO_INBUF_SI_ADDR));

    vAprocReg_Write (APROC_REG_IOBUF_OUT_START, 
        u4GetAprocMemoryMap(APROC_MEMMAP_ADSP_CMPT)+(4*ADDR_CMPT_APROC_IO_OUTBUF_ADDR));
    vAprocReg_Write (APROC_REG_IOBUF_OUT_END, 32);
    vAprocReg_Write (APROC_REG_IOBUF_OUT_SIZE, 2 * 256 * 4);
    vAprocReg_Write (APROC_REG_IOBUF_OUT_SI_BASE, 
        u4GetAprocMemoryMap(APROC_MEMMAP_ADSP_DRAM)+(4*ADDR_APROC_IO_OUTBUF_SI_ADDR));
    
    vAprocCmdOpen (APROC_SIG_IDX_POSTPROC);
    vAprocWaitCmdDone ();
}

//-----------------------------------------------------------------------------
/** vAprocPostProc_Close
 *  Audio processor close post-processing
 *
 *  @param           
 *  @retval 
 */
//-----------------------------------------------------------------------------
void vAprocPostProc_Close (void)
{
    if (!_gu4AprocEnable)
    {
        return;
    }

    if (_gu4AprocPostProcControl == APROC_POSTPROC_CLOSE)
    {
        return;
    }

    LOG (5, "====> vAprocPostProc_Close\n");
    vAprocCmdClose (APROC_SIG_IDX_POSTPROC);
    vAprocWaitCmdDone ();
    _gu4AprocPostProcControl = 0;
}

typedef struct {
    UINT32 u4Str;// start address
    UINT32 u4End; // total block number
    UINT32 u4Size; // bytes in one block per channel
    UINT32 u4Ch; // channel number
    UINT32 u4Wcurp; // Write port, current pointer
    UINT32 u4Rcurp; // Read port, current pointer
    UINT32 u4Wp; // write pointer
    UINT32 u4Rp; // read pointer
    UINT32 u4SiBase; // sideinformation base address
} _IO_BUFFER_T;

_IO_BUFFER_T _gsAprocUploadBufInfoNew;
_IO_BUFFER_T _gsAprocUpload2BufInfoNew;

void vAprocUploadUpdateWp(UINT32 u4Wp)
{
    _gsAprocUploadBufInfoNew.u4Wp = u4Wp;
}

void vAprocCpuLoadInit(void)
{
    _gsAprocUploadBufInfoNew.u4Str = _u4DspCmptBuf[AUD_DSP0][TYPE_APROC_IDX] + 
        u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD0_START));
    _gsAprocUploadBufInfoNew.u4End = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD0_END));
    _gsAprocUploadBufInfoNew.u4Size = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD0_SIZE));
    _gsAprocUploadBufInfoNew.u4Ch = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD0_CH));

    _gsAprocUploadBufInfoNew.u4Rp = 0;    
    _gsAprocUploadBufInfoNew.u4Wp = 0;
    _gsAprocUploadBufInfoNew.u4Rcurp = 0;    
    _gsAprocUploadBufInfoNew.u4Wcurp = 1;
}

UINT32 u4AprocGetUploadFIFOStart(void)
{
	return _gsAprocUploadBufInfoNew.u4Str;
}

UINT32 u4AprocGetUploadFIFOEnd(void)
{
	// Come from APROC uploader initial, it should equal to 3
	return _gsAprocUploadBufInfoNew.u4End;
}

UINT32 u4AprocGetUploadBlkSz(void)
{
	// Come from APROC uploader initial, it should equal to (768 * 4) bytes 
	return _gsAprocUploadBufInfoNew.u4Size;
}
UINT32 u4AprocGetUploadCh(void)
{
	// Come from APROC uploader initial
	return _gsAprocUploadBufInfoNew.u4Ch;
}

UINT32 u4AprocGetUploadWp(void)
{
	return _gsAprocUploadBufInfoNew.u4Wp;
}

void vAprocUploadReset(void)
{
	// Request ARM9 to reset mixsound buffer info.
    vAprocCmdSet(APROC_SIG_IDX_UPLOAD0, APROC_SIG_SET_RESET, 0);    
}

void vAprocUploadEnable(BOOL fgEnable)
{
	// Send MixSound Enable Command to ARM9    
    vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD0_ENABLE), fgEnable);
}

void vAprocUploadUpdateRp(UINT32 u4Rp)
{
	_gsAprocUploadBufInfoNew.u4Rp = u4Rp;
	vAprocCmdSet(APROC_SIG_IDX_UPLOAD0, APROC_SIG_SET_UPDATE_RP, u4Rp);
}
//For Upload2()
void vAprocUpload2UpdateWp(UINT32 u4Wp)
{
    _gsAprocUpload2BufInfoNew.u4Wp = u4Wp;
}

void vAprocCpuLoad2Init(void)
{
    _gsAprocUpload2BufInfoNew.u4Str = _u4DspCmptBuf[AUD_DSP0][TYPE_APROC_IDX] + 
        u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD2_START));
    _gsAprocUpload2BufInfoNew.u4End = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD2_END));
    _gsAprocUpload2BufInfoNew.u4Size = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD2_SIZE));
    _gsAprocUpload2BufInfoNew.u4Ch = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD2_CH));

    _gsAprocUpload2BufInfoNew.u4Rp = 0;    
    _gsAprocUpload2BufInfoNew.u4Wp = 0;
    _gsAprocUpload2BufInfoNew.u4Rcurp = 0;    
    _gsAprocUpload2BufInfoNew.u4Wcurp = 1;
}

UINT32 u4AprocGetUpload2FIFOStart(void)
{
	return _gsAprocUpload2BufInfoNew.u4Str;
}

UINT32 u4AprocGetUpload2FIFOEnd(void)
{
	// Come from APROC uploader initial, it should equal to 3
	return _gsAprocUpload2BufInfoNew.u4End;
}

UINT32 u4AprocGetUpload2BlkSz(void)
{
	// Come from APROC uploader initial, it should equal to (768 * 4) bytes 
	return _gsAprocUpload2BufInfoNew.u4Size;
}
UINT32 u4AprocGetUpload2Ch(void)
{
	// Come from APROC uploader initial
	return _gsAprocUpload2BufInfoNew.u4Ch;
}

UINT32 u4AprocGetUpload2Wp(void)
{
	return _gsAprocUpload2BufInfoNew.u4Wp;
}

void vAprocUpload2Reset(void)
{
	// Request ARM9 to reset mixsound buffer info.
    vAprocCmdSet(APROC_SIG_IDX_UPLOAD2, APROC_SIG_SET_RESET, 0);    
}

void vAprocUpload2Enable(BOOL fgEnable)
{
	// Send MixSound Enable Command to ARM9    
    vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD2_ENABLE), fgEnable);
}

void vAprocUpload2UpdateRp(UINT32 u4Rp)
{
	_gsAprocUpload2BufInfoNew.u4Rp = u4Rp;
	vAprocCmdSet(APROC_SIG_IDX_UPLOAD2, APROC_SIG_SET_UPDATE_RP, u4Rp);
}

#endif //CC_AUD_ARM_SUPPORT


