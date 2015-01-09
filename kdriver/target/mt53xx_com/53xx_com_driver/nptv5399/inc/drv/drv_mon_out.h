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
 /*************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: drv_regen.h $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *
 *
 * Author:
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Date: 2015/01/09 $
 *
 * $Revision: #1 $
*************************************************************************/
#ifndef _DRV_MON_OUT_H_
#define _DRV_MON_OUT_H_

#include "general.h"
#include "video_def.h"

#define     MON_OUT_DRAM_FRAME_NUM   2

typedef enum {
    E_TTT_2D_IN          =0,
    E_TTT_PR_IN          =1,
    E_TTT_SG_IN          =2,
    E_TTT_SBS_IN		 =3,
    E_TTT_TB_IN			 =4,
}E_MON_OUT_TTT_CONFIG;



typedef enum {
    E_MON_OUT_MIXER         =0,
    E_MON_OUT_MAIN_FSC_OUT  =1,
    E_MON_OUT_SUB_FSC_OUT   =2,
    E_MON_OUT_MAIN_MIB_OUT  =3,
    E_MON_OUT_SUB_MIB_OUT   =4,
    E_MON_OUT_MAIN_PREDOWN  =5,
    E_MON_OUT_SUB_PREDOWN   =6,   
    E_MON_OUT_PANEL         =7,
    E_FSC_SUB_TO_TVE        =8,
}E_MON_OUT_IN_SELECT;

typedef enum {
    E_MON_OUT_TVE	        =0,
	E_MON_OUT_VENC			=1,
	E_MON_OUT_GFX			=2,
}E_MON_OUT_OUTPUT_SELECT;


typedef enum {    
    E_MON_OUT_WITH_ALL_OSD  =0,
    E_MON_OUT_WITH_ONE_OSD  =1,
    E_MON_OUT_WITH_NO_OSD   =2,
 }E_MON_OUT_IN_SELECT_2;


#define  FRAME_START_AT_FALLING      0
#define  FRAME_START_AT_RISING       1



#define MONOUT_WRITE_FLD(reg,value,fld) vIO32WriteFldAlign(reg, value, fld)
#define MONOUT_READ_FLD(reg,fld) IO32ReadFldAlign(reg,fld)

#define getMonOutInfo()       (&_rMonitorOutInfo)
#define getVrmMonOutInfo()    (&_vrm_mon_out_info)


void vMonOutNormProc(void);
void vMonOutModuleCfg(void);
void vDrvMonOutInit(void);
void vMonOutUpdatePreDownParameter(void);
void vMonOutUpdateDramParameter( void );
void vMonOutSetMuxSelIn(E_MON_OUT_IN_SELECT MonOutMuxIn);
UINT8 vMonOutGetMuxSelIn(void);
void vMonOutSetOSDSelIn(E_MON_OUT_IN_SELECT_2 MonOutOSDIn);
void vMonOutSetOutput(E_MON_OUT_OUTPUT_SELECT MonOutOutput);
void vMonOutSetOutputRes(UINT16 width ,UINT16 height);
void vMonOutGetOutputRes(UINT16* width ,UINT16* height);
void vMonOutSetTveFormat(UINT8 TimeGenSel);
void vMonOutSetEnable(UINT8 Enable);
UINT8 vMonOutGetEnable(void);
void vMonOutSetRegen(VDP_DISP_REGION_T rRegion);
UINT8 u1IsMonOutVenc(void);
void vMonOutGetVideoBlackBar(UINT32 *top,UINT32 *bottom,UINT32 *left,UINT32 *right);

UINT8 vMonOutGetDramReadEnable(void);
UINT8 vMonOutGetDramWriteEnable(void);
UINT8 u1MonOutGetFldInfo(void);
void vMonOutSetDotByDotOutput(BOOL bflag);
BOOL vMonOutGetDotByDotOutput(void);
#endif

