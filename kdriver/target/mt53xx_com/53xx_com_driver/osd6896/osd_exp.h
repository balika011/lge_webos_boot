

#ifndef __OSD_EXP_H__
#define __OSD_EXP_H__

#include "osd_drvif.h"

typedef enum
{
    OSD_EXP_RET_OK = 0,
    OSD_EXP_RET_INVALID_ARGV = -1,
    OSD_EXP_RET_OUTOF_RESOURCE = -2,
    OSD_EXP_RET_CALLBACK_REGISTED = -3,
}OSD_EXP_RET_T;

typedef struct
{
    INT32 i4Identity;
    VOID (*pfnVsyncIsr)(VOID);
    VOID (*pfnResumeSuspend)(BOOL fgSuspend);
    VOID (*pfnTimingChanged)(VOID);
}
OSD_EXP_CALLBACK_T;

OSD_EXP_RET_T OSD_EXP_GenCallback(INT32 *pi4Identity);
OSD_EXP_RET_T OSD_EXP_RegisterCallback(OSD_EXP_CALLBACK_T *pfnCb);
OSD_EXP_RET_T OSD_EXP_RemoveCallback(INT32 i4Identity);
OSD_EXP_RET_T OSD_EXP_HandleVSync(VOID);
OSD_EXP_RET_T OSD_EXP_HandleResumeSuspend(BOOL fgSuspend);

VOID OSD_EXP_BypPQPath(VOID);
VOID OSD_EXP_EnablePQPath(VOID);
VOID OSD_EXP_BypGFXPath(VOID);
VOID OSD_EXP_EnableGFXPath(VOID);

BOOL OSD_EXP_IS_GFX_4K2K(VOID);
BOOL OSD_EXP_IS_GFX_FHD(VOID);
BOOL OSD_EXP_IS_GFX(VOID);

#endif
