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
#ifndef _VDO_RM_H_
#define _VDO_RM_H_

#include "general.h"
#include "video_def.h"
#include "vdp_if.h"

#define DEF_TO_STR(a) #a

#define VRM_PATH_MAIN                            0x0
#define VRM_PATH_SUB                            (0x1)
#define VRM_PATH_MON                            (0x2)

#define VRM_EVENT_NONE                           0x0
#define VRM_EVENT_BY_CLI                        (1<<0)
#define VRM_EVENT_BY_VDP                        (1<<1)
#define VRM_EVENT_BY_TDTV                       (1<<2)
#define VRM_EVENT_BY_FORCE_PDS                  (1<<3)
#define VRM_EVENT_BY_VDO_MODCHG                 (1<<4)
#define VRM_EVENT_BY_DISPMODE_CHG               (1<<5)
#define VRM_EVENT_BY_MIB                        (1<<6)
#define VRM_EVENT_BY_SCPOS                      (1<<7)
#define VRM_EVENT_BY_B2R                        (1<<8)
#define VRM_EVENT_BY_PANEL                      (1<<9)
#define VRM_EVENT_BY_RECAL                      (1<<10)
#define VRM_EVENT_BY_DISPLAY                    (1<<11)
#define VRM_EVENT_BY_APP                        (1<<12)

#define VRM_IMPORT_WAIT_NONE                     0x0
#define VRM_IMPORT_WAIT_FLAG_B2R                (1<<1)
#define VRM_IMPORT_WAIT_FLAG_PDS                (1<<2)
#define VRM_IMPORT_WAIT_FLAG_REGEN              (1<<3)
#define VRM_IMPORT_WAIT_FLAG_MIB                (1<<4)
#define VRM_IMPORT_WAIT_FLAG_FSC_REGEN          (1<<5)
#define VRM_IMPORT_WAIT_FLAG_FSC                (1<<6)
#define VRM_IMPORT_WAIT_FLAG_TRIG_START         (1<<8)
#define VRM_IMPORT_WAIT_FLAG_TRIG_END           (1<<9)

#define VRM_APP_NONE                             0x0
#define VRM_APP_POP_PIP                         (0x1)
#define VRM_APP_VIDEO_ENC                       (0x2)
#define VRM_APP_GRAPHIC_DUMP                    (0x4)
#define VRM_APP_SCART_OUT                       (0x8)

#define VRM_STAGE_PDS                            0x0
#define VRM_STAGE_DI                            (1<<1)
#define VRM_STAGE_FSC                           (1<<2)
#define VRM_STAGE_OSTG                          (1<<3)
#define VRM_STAGE_MON                           (1<<4)

#define fireVRMModeChange(bPath, event)   vVRMSetEventFlg(bPath, event);

#define VRM_TURNOFF_IMPORT_AT_3D

//for mt5399 mon_out porting 
//start-----
#define   VRM_MON_OUT_EVENT_NONE                (0x00)
#define   VRM_MON_OUT_IN_SEL_CHG                (1<<1)
#define   VRM_MON_OUT_REGEN_CHG                 (1<<2)
#define   VRM_MON_OUT_TIME_GEN_CHG              (1<<3)
#define   VRM_MON_OUT_DOWN_SC_CHG               (1<<4)
#define   VRM_MON_OUT_EVENT_BY_CLI              (1<<5)
#define   VRM_MON_OUT_OUT_SEL_CHG               (1<<6)
#define   VRM_MON_OUT_TTT_FMT_CHG               (1<<7)
#define   VRM_MON_OUT_INPUT_RES_CHG             (1<<8)
#define   VRM_MON_OUT_OUTPUT_RES_CHG            (1<<9)

//end ------

typedef struct
{
    UINT16 u2VRMID;
    UINT16 u2X;
    UINT16 u2Y;
    UINT16 u2Width;
    UINT16 u2Height;
    UINT16 u2HTotal;
    UINT16 u2VTotal;
    UINT16 u2FrameRate;
    UINT16 u2ScanMode;
}VRM_INFO_T;

typedef enum
{
    VRM_MODULE_OMUX,            //Output Mux
    VRM_MODULE_CB,              //Checker Board
    VRM_MODULE_DQ,              //De-quincunx
    VRM_MODULE_TTT,             //3D to 2D
    VRM_MODULE_PDS,             //Pre down
    VRM_MODULE_REGEN,           //Regen
    VRM_MODULE_MIB,             //MIB    
    VRM_MODULE_FSC_REGEN,       //FSC REGEN
    VRM_MODULE_FSC,             //Front scaler
    VRM_MODULE_MIXER,           //Main/Sub Mixer
    VRM_MODULE_MJC,             //MJC
    VRM_MODULE_PSC,             //Post scaler
    VRM_MODULE_PSC_MIXER,       //Post scaler Mixer
    VRM_MODULE_OSTG,            //Output stage
    VRM_MODULE_MAX,
}VRM_MODULE_E;



typedef enum
{
    IMPORT_MODULE_NONE,
    IMPORT_MODULE_B2R,
    IMPORT_MODULE_PDS,
    IMPORT_MODULE_REGEN,
    IMPORT_MODULE_MIB,
    IMPORT_MODULE_FSC,
    IMPORT_MODULE_UNKNOWN,
}IMPORT_MODULE_E;

typedef struct 
{
    IMPORT_MODULE_E forcedImportModule;
    IMPORT_MODULE_E importModule;
    UINT32 importWaitFlag;
}IMPORT_INFO_T;

typedef enum
{
    OVSN_MODULE_REGEN,
    OVSN_MODULE_FSC_REGEN,
}OVSN_MODULE_E;

typedef enum
{
    FORCED_OVSN_MODULE_AUTO,
    FORCED_OVSN_MODULE_REGEN,
    FORCED_OVSN_MODULE_FSC_REGEN,
}FORCED_OVSN_MODULE_E;

typedef enum
{
    SET_RES_INPUT,
    SET_RES_OUTPUT,
    SET_RES_BOTH,
}SET_RES_STATUS_TYPE_E;

typedef struct 
{
    UINT32 u4Width;
    UINT32 u4Height;
}APP_RES_T;

void vVRMSwInit(void);
void vVrmMonOutSwInit(void);
void vVRMLoop(UINT8 bPath);
void vVRM_MonOutLoop( void );

VRM_INFO_T* VRMGetVRMInfo(UINT8 bPath, VRM_MODULE_E moduleID);
void VRMGetModuleVRMInfo(UINT8 bPath, VRM_MODULE_E moduleID, VRM_INFO_T* in, VRM_INFO_T* out);
void vVRMDumpVRMInfo(UINT8 bPath, VRM_MODULE_E moduleID);
void vVRMDumpAllVRMInfo(void);

//for VRM mode change flag
BOOL fgVRMIsEventFlgSet(UINT8 bPath, UINT32 u4arg);
UINT32 vVRMGetEventFlg(UINT8 bPath);
VOID vVRMSetEventFlg(UINT8 bPath,UINT32 u4arg);
VOID vVRMClrEventFlg(UINT8 bPath);

void vVRMSetMonOutEventFlg( UINT32 u4MonOutFlag );
UINT32 vVRMGetMonOutEventFlg( void );

//for CLI
void vVRMTriggerUpdateByCli(UINT8 bPath);

//for comparing if VRM info is changed or not
BOOL fgVRMIsEqual(VRM_INFO_T* vrmInfo1, VRM_INFO_T* vrmInfo2);

//for update vrm info of all module
void vVRMUpdateResolution(UINT8 bPath);
//for update resoluton of each module
void vVRMUpdate121Resolution(UINT8 bPath);
void vVRMUpdateOMUXResolution(UINT8 bPath);
void vVRMUpdateCBResolution(UINT8 bPath);
void vVRMUpdateDQResolution(UINT8 bPath);
void vVRMUpdateTTTResoluton(UINT8 bPath);
void vVRMUpdatePredownResolution(UINT8 bPath);
void vVRMUpdateRegenResolution(UINT8 bPath);
void vVRMUpdateMIBResolution(UINT8 bPath);
void vVRMUpdateMIBResolutionWithoutOvsn(UINT8 bPath);
void vVRMUpdateMIBResolutionWithOvsn(UINT8 bPath);
void vVRMUpdateFSCRegenResolution(UINT8 bPath);
void vVRMUpdateFSCResolutionWithOvsn(UINT8 bPath);
void vVRMUpdateFSCResolutionWithoutOvsn(UINT8 bPath);
void vVRMUpdateFSCResolution(UINT8 bPath);
void vVRMUpdateMixerResolution(UINT8 bPath);
void vVRMUpdateMJCResolution(UINT8 bPath);
void vVRMUpdatePSCResolution(UINT8 bPath);
void vVRMUpdateOSTGResolution(UINT8 bPath);
UINT8 fgVRMUpdateDIMaxOutputWidth(UINT8 bPath);
UINT32 u4VrmGetDIMaxOutWidth(UINT8 bPath);

void vVrmAllImportEn(UINT8 bPath, UINT8 bOnOff);
void vVrmOnVdoModeChange(UINT8 bPath);
void vVrmOnVdoModeChangeDone(UINT8 bPath);
void vDrvSetImportTriggerModule(UINT8 bPath, IMPORT_MODULE_E module);
IMPORT_MODULE_E vDrvGetImportTriggerModule(UINT8 bPath);
void vDrvSetForcedImportTriggerModule(UINT8 bPath, IMPORT_MODULE_E module);
IMPORT_MODULE_E eDrvGetForcedImportTriggerModule(UINT8 bPath);
void vDrvFireB2RImportTrigger(UINT8 bPath);
void vDrvFirePDSImportTrigger(UINT8 bPath);
void vDrvFireRegenImportTrigger(UINT8 bPath);
void vDrvFireMIBImportTrigger(UINT8 bPath);
void vDrvFireFSCImportTrigger(UINT8 bPath);
void vDrvFireImportTrigger(UINT8 bPath);
void vDrvSetImportWaitFlag(UINT8 bPath, UINT32 u4Flag);
void vDrvClrImportWaitFlag(UINT8 bPath, UINT32 u4Flag);
UINT32 vDrvGetImportWaitFlag(UINT8 bPath);
void vDrvSetImportWaiting(UINT8 bPath);

void vDrvVrmSetAppStage(void);

void vDrvSetPDSTriggerFlg(UINT8 u1Path, UINT8 fgOnOff);
UINT8 vDrvQueryPDSTriggerFlg(UINT8 u1Path);

void vDrvVrmSetAppFlag(UINT32 u4Flag);
void vDrvVrmClrAppFlag(UINT32 u4Flag);
BOOL bDrvVrmIsAppFlagSet(UINT32 u4Flag);
BOOL bDrvVrmIsAppFifoDramLimit(UINT8 bPath);
UINT32 u4DrvVrmGetAppFlag(void);
void vDrvVrmSetAppRes(UINT32 u4AppType, APP_RES_T rVencRes);
void vDrvVrmGetAppRes(UINT32 u4AppType, APP_RES_T *rVencRes);
void vDrvVRMResume(void);


void vVRMSetImportDelay(UINT8 bPath, UINT8 u1Delay); 
UINT8 vVRMGetImportDelay(UINT8 bPath); 

//determine ovsn module
void vVrmSetForcedOvsnModule(UINT8 bPath, FORCED_OVSN_MODULE_E ovsnModule);
OVSN_MODULE_E eVrmGetOvsnModule(UINT8 bPath);
void vVrmUpdateOvsnModule(UINT8 bPath);

/******B2R interface**************/
UINT8 bVRMReadyForB2R(UINT8 bPath);
void vVRMB2RTrigger(UINT8 bPath);
void vVRMSetB2RTriggerStatus(UINT8 bPath, UINT8 bStatus);
UINT8 bVRMUpdatingSrcRegDone(UINT8 bPath);

#endif
