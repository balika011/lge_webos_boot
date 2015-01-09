
#include "osd_exp.h"
#include "osd_hw.h"
/***************************************************************************************************************************
****************************************************************************************************************************
//The implementation for OSD callback
****************************************************************************************************************************
***************************************************************************************************************************/

typedef struct
{
    BOOL fgRegisted;
    OSD_EXP_CALLBACK_T fnCallback;
}
REGISTER_CALLBACK_T;

static REGISTER_CALLBACK_T _rRegisterCallback[] = {
    {
        .fgRegisted = FALSE,
    },
    {
        .fgRegisted = FALSE,
    },
    {
        .fgRegisted = FALSE,
    }
};

static UINT32 _u4CallbackMaxNumber = sizeof(_rRegisterCallback) / sizeof(REGISTER_CALLBACK_T);
static INT32 _i4Identity = 0;

static REGISTER_CALLBACK_T *OSD_Internal_GetIdleRegisterCallback(VOID)
{
    INT32 i;
    
    for(i = 0; i < _u4CallbackMaxNumber; i++)
    {
        if(_rRegisterCallback[i].fgRegisted == FALSE)
        {
            return (&_rRegisterCallback[i]);
        }
    }

    return NULL;
}


static OSD_EXP_RET_T OSD_Internal_CheckCallbackRegistered(INT32 i4Identity)
{
    INT32 i;
    
    for(i = 0; i < _u4CallbackMaxNumber; i++)
    {
        if(_rRegisterCallback[i].fgRegisted == TRUE)
        {
            if(_rRegisterCallback[i].fnCallback.i4Identity == i4Identity)
            {
                return OSD_EXP_RET_CALLBACK_REGISTED;
            }
        }
    }

    return OSD_EXP_RET_OK;
}

OSD_EXP_RET_T OSD_EXP_GenCallback(INT32 *pi4Identity)
{
    INT32 i4Identity;
    
    if(pi4Identity == NULL)
    {
        return OSD_EXP_RET_INVALID_ARGV;
    }

    while(1)
    {
        i4Identity = _i4Identity++;
        if(OSD_Internal_CheckCallbackRegistered(i4Identity) == OSD_EXP_RET_OK)
        {
            break;
        }
    }

    *pi4Identity = i4Identity;

    return OSD_EXP_RET_OK; 
}


OSD_EXP_RET_T OSD_EXP_RegisterCallback(OSD_EXP_CALLBACK_T *pfnCb)
{
    OSD_EXP_RET_T rRet;
    REGISTER_CALLBACK_T *prRegCb = NULL;
    
    if(pfnCb == NULL)
    {
        return OSD_EXP_RET_INVALID_ARGV;
    }

    rRet = OSD_Internal_CheckCallbackRegistered(pfnCb->i4Identity);
    if(rRet != OSD_EXP_RET_OK)
    {
        return rRet;
    }

    prRegCb = OSD_Internal_GetIdleRegisterCallback();
    if(prRegCb == NULL)
    {
        return OSD_EXP_RET_OUTOF_RESOURCE;
    }

    prRegCb->fgRegisted = TRUE;
    prRegCb->fnCallback = *pfnCb;

    return OSD_EXP_RET_OK;
}

OSD_EXP_RET_T OSD_EXP_RemoveCallback(INT32 i4Identity)
{
    INT32 i;
    
    for(i = 0; i < _u4CallbackMaxNumber; i++)
    {
        if(_rRegisterCallback[i].fgRegisted == TRUE)
        {
            if(_rRegisterCallback[i].fnCallback.i4Identity == i4Identity)
            {
                _rRegisterCallback[i].fgRegisted = FALSE;
                return OSD_EXP_RET_OK;
            }
        }
    }

    return OSD_EXP_RET_INVALID_ARGV;
}

OSD_EXP_RET_T OSD_EXP_HandleVSync(VOID)
{
    INT32 i;
    
    for(i = 0; i < _u4CallbackMaxNumber; i++)
    {
        if(_rRegisterCallback[i].fgRegisted == TRUE)
        {
            if(_rRegisterCallback[i].fnCallback.pfnVsyncIsr != NULL)
            {
                _rRegisterCallback[i].fnCallback.pfnVsyncIsr();
            }
        }
    }

    return OSD_EXP_RET_OK;    
}

OSD_EXP_RET_T OSD_EXP_HandleResumeSuspend(BOOL fgSuspend)
{
    INT32 i;
    
    for(i = 0; i < _u4CallbackMaxNumber; i++)
    {
        if(_rRegisterCallback[i].fgRegisted == TRUE)
        {
            if(_rRegisterCallback[i].fnCallback.pfnResumeSuspend != NULL)
            {
                _rRegisterCallback[i].fnCallback.pfnResumeSuspend(fgSuspend);
            }
        }
    }

    return OSD_EXP_RET_OK;    
}

OSD_EXP_RET_T OSD_EXP_HandleTimingChanged(VOID)
{
    INT32 i;
    
    for(i = 0; i < _u4CallbackMaxNumber; i++)
    {
        if(_rRegisterCallback[i].fgRegisted == TRUE)
        {
            if(_rRegisterCallback[i].fnCallback.pfnTimingChanged != NULL)
            {
                _rRegisterCallback[i].fnCallback.pfnTimingChanged();
            }
        }
    }

    return OSD_EXP_RET_OK;     
}

/***************************************************************************************************************************
****************************************************************************************************************************
//The implementation for OSD PQ path and Gfx path
****************************************************************************************************************************
***************************************************************************************************************************/
typedef struct
{
    UINT32 : 1;
    UINT32 u4BotMaskSel : 1;
    
    UINT32 u4Mix11TopMixBg : 1;
    UINT32 u4Mix11MaskOrEn : 1;
    UINT32 u4Mix11Bypass : 1;
    
    UINT32 u4Mix12TopMixBg : 1;
    UINT32 u4Mix12MaskOrEn : 1;
    UINT32 u4Mix12Bypass : 1;
    
    UINT32 u4Mix2TopMixBg : 1;
    UINT32 u4Mix2MaskOrEn : 1;
    UINT32 u4Mix2Bypass : 1;
    
    UINT32 u4Mix3TopMixBg : 1;
    UINT32 u4Mix3MaskOrEn : 1;
    UINT32 u4Mix3Bypass : 1;

    UINT32 u4RegMix3SignEn : 1;
    UINT32 u4RegMix2SignEn : 1;
    UINT32 u4RegMix1SignEn : 1;

    UINT32 u4RegMixSignAutoOff : 1;
    UINT32 : 2;
    UINT32 u4RegMix3BotAlpha : 8;

    UINT32 : 4;
}
OSD_MIX_0_FIELD_T;

typedef struct
{
    UINT32 u4RegMix11SelBotAlpha : 1;
    UINT32 u4RegMix12SelBotAlpha : 1;
    UINT32 u4RegMix2SelBotAlpha : 1;
    UINT32 u4RegMix3SelBotAlpha : 1;
    UINT32 : 4;
    UINT32 u4RegMix11BotAlpha : 8;
    UINT32 u4RegMix12BotAlpha : 8;
    UINT32 u4RegMix2BotAlpha : 8;
}
OSD_MIX_1_FILED_T;

typedef union
{
    UINT32 u4Value;
    OSD_MIX_0_FIELD_T rFiled;
}OSD_MIX_0_T;

typedef union
{
    UINT32 u4Value;
    OSD_MIX_1_FILED_T rFiled;
}OSD_MIX_1_T;

#define OSD_MIX_0_REG (0xF0027984)
#define OSD_MIX_1_REG (0xF0027988)
#define GFX_MIX_0_REG (0xF0027A48)
#define GFX_MIX_1_REG (0xF0027A4C)
#define OSD_MIX_0_READ32()            IO_READ32(OSD_MIX_0_REG,(0))
#define OSD_MIX_1_READ32()            IO_READ32(OSD_MIX_1_REG,(0))
#define GFX_MIX_0_READ32()            IO_READ32(GFX_MIX_0_REG,(0))
#define GFX_MIX_1_READ32()            IO_READ32(GFX_MIX_1_REG,(0))
#define OSD_MIX_0_WRITE32(value)      IO_WRITE32(OSD_MIX_0_REG,(0),(value))
#define OSD_MIX_1_WRITE32(value)      IO_WRITE32(OSD_MIX_1_REG,(0),(value))
#define GFX_MIX_0_WRITE32(value)      IO_WRITE32(GFX_MIX_0_REG,(0),(value))
#define GFX_MIX_1_WRITE32(value)      IO_WRITE32(GFX_MIX_1_REG,(0),(value))

VOID OSD_EXP_BypPQPath(VOID)
{
    OSD_MIX_0_T rMix0;

    rMix0.u4Value = OSD_MIX_0_READ32();
    rMix0.rFiled.u4Mix11Bypass = 1;
    rMix0.rFiled.u4Mix11TopMixBg = 1;
    rMix0.rFiled.u4Mix12Bypass = 1;
    rMix0.rFiled.u4Mix12TopMixBg = 1;
    rMix0.rFiled.u4Mix2Bypass = 1;
    rMix0.rFiled.u4Mix2TopMixBg = 1;
    rMix0.rFiled.u4Mix3Bypass = 1;
    rMix0.rFiled.u4Mix3TopMixBg = 1;    

    OSD_MIX_0_WRITE32(rMix0.u4Value);
}

VOID OSD_EXP_EnablePQPath(VOID)
{
    OSD_MIX_0_T rMix0;

    rMix0.u4Value = OSD_MIX_0_READ32();
    rMix0.rFiled.u4Mix11Bypass = 0;
    rMix0.rFiled.u4Mix12Bypass = 0;
    rMix0.rFiled.u4Mix2Bypass = 0;
    rMix0.rFiled.u4Mix3Bypass = 0;
    
    OSD_MIX_0_WRITE32(rMix0.u4Value);
}

VOID OSD_EXP_BypGFXPath(VOID)
{
    OSD_MIX_0_T rMix0;

    rMix0.u4Value = GFX_MIX_0_READ32();
    rMix0.rFiled.u4Mix11Bypass = 1;
    rMix0.rFiled.u4Mix11TopMixBg = 1;
    rMix0.rFiled.u4Mix12Bypass = 1;
    rMix0.rFiled.u4Mix12TopMixBg = 1;
    rMix0.rFiled.u4Mix2Bypass = 1;
    rMix0.rFiled.u4Mix2TopMixBg = 1;
    rMix0.rFiled.u4Mix3Bypass = 1;
    rMix0.rFiled.u4Mix3TopMixBg = 1;    

    GFX_MIX_0_WRITE32(rMix0.u4Value);
}

VOID OSD_EXP_EnableGFXPath(VOID)
{
    OSD_MIX_0_T rMix0;

    rMix0.u4Value = GFX_MIX_0_READ32();
    rMix0.rFiled.u4Mix11Bypass = 0;
    rMix0.rFiled.u4Mix12Bypass = 0;
    rMix0.rFiled.u4Mix2Bypass = 0;
    rMix0.rFiled.u4Mix3Bypass = 0;
    
    GFX_MIX_0_WRITE32(rMix0.u4Value);
}

#define OSD_TIMGEN_SRC_REG     (0xF0025350)
#define OSD_TIMGEN_SRC_BIT     13
#define OSD_TIMGEN_SRC_MARKS   0x3
static INT32 OSD_EXP_TimGenVal(VOID)
{
    UINT32 u4Value = IO_READ32(OSD_TIMGEN_SRC_REG, 0);
    u4Value = (u4Value >> OSD_TIMGEN_SRC_BIT);
    return (u4Value & OSD_TIMGEN_SRC_MARKS);
}
BOOL OSD_EXP_IS_GFX_4K2K(VOID)
{
    if(OSD_EXP_TimGenVal() == 0x2 && OSD_BASE_GetDisplayWidth() == 3840 && OSD_BASE_GetDisplayHeight() == 2160)
    {
        return TRUE;
    }
    return FALSE;
}
BOOL OSD_EXP_IS_GFX_FHD(VOID)
{
    if(OSD_EXP_TimGenVal() == 0x2 && OSD_BASE_GetDisplayWidth() == 1920 && OSD_BASE_GetDisplayHeight() == 1080)
    {
        return TRUE;
    }
    return FALSE;
}
BOOL OSD_EXP_IS_GFX(VOID)
{
    if(OSD_EXP_TimGenVal() == 0x2)
    {
        return TRUE;
    }
    return FALSE;
}
