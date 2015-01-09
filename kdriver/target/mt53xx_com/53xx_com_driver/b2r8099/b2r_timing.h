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
 * $RCSfile: b2r_hw_v1.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file b2r_timing.h
 *  This header file of b2r_timing.h.
 */


#ifndef _B2R_TIMING_H_
#define _B2R_TIMING_H_

/*
In the spec, the standard frame rate are 24, 24/1.001, 25, 30, 30/1.001, 50, 60 and 60/1.001.
but here, 
  1)also support  48, 48/1.001, 100, 120 and 120/1.001
*/

typedef struct 
{
    CHAR*  acName;      
    UINT16 u2HTotal;        
    UINT16 u2VTotal;        
    UINT32 u4Rate;      
    UINT32 u4Scale;     
    UINT32 u4Clock;     
} __TIMING_TBL_T;

typedef struct _TIMING_MATRIX_T
{
    UINT16 u2MaxHAct;        
    UINT16 u2MaxVAct;        
    __TIMING_TBL_T *ptTbl;
    UINT16 u2Size;     
} __TIMING_MATRIX_T;

//progressive timing table
//720x480 spec 
static __TIMING_TBL_T _720_480_standard [] =
{
    {"720x480/120P",   858,  525, 120,   1,    54054000},
    {"720x480/119.88P",858,  525, 120000,1001, 54000000},
    {"720x480/60P",    858,  525, 60,    1,    27027000},
    {"720x480/59.94P", 858,  525, 60000, 1001, 27000000},
    {"720x480/48P",    858,  525, 48,    1,    21621600},   
    {"720x480/47.95P", 858,  525, 48000, 1001, 21600000},   
    {"720x480/30P",    858,  525, 30,    1,    27027000},   //use horizontal de ratio
    {"720x480/29.97P", 858,  525, 30000, 1001, 27000000},   //use horizontal de ratio
    {"720x480/24P",    858,  525, 24,    1,    21621600},   //use horizontal de ratio
    {"720x480/23.97P", 858,  525, 24000, 1001, 21600000},   //use horizontal de ratio
    {"720x480/15P",    858,  525, 15,    1,    13513500}
};

static __TIMING_TBL_T _720_576_standard [] =
{
    {"720x576/100P",   864, 625, 100,   1,    54000000},
    {"720x576/50P",    864, 625, 50,    1,    27000000},
    {"720x576/25P",    864, 625, 25,    1,    27000000},     //use horizontal de ratio
    {"720x576/12P",    864, 625, 12,    1,    13500000},
    {"720x576/15P",    864, 625, 15,    1,    8100000}
};


//720P spec from smpte-296m-2001
static __TIMING_TBL_T _1280_720_standard [] =
{
    {"1280x720/120P",   1650, 750, 120,   1,    148500000},
    {"1280x720/119.88P",1650, 750, 120000,1001, 148352000},
    {"1280x720/100P",   1980, 750, 100,   1,    148500000},
    {"1280x720/60P",    1650, 750, 60,    1,    74250000},
    {"1280x720/59.94P", 1650, 750, 60000, 1001, 74176000},
    {"1280x720/50P",    1980, 750, 50,    1,    74250000},
    {"1280x720/48P",    4125, 750, 48,    1,    74250000},
    {"1280x720/47.95P", 4125, 750, 48000, 1001, 74176000},
    {"1280x720/30P",    3300, 750, 30,    1,    74250000},
    {"1280x720/29.97P", 3300, 750, 30000, 1001, 74176000},
    {"1280x720/25P",    3960, 750, 25,    1,    74250000},
    {"1280x720/24P",    4125, 750, 24,    1,    74250000},
    {"1280x720/12P",    4125, 750, 12,    1,    37125000},
    {"1280x720/23.97P", 4125, 750, 24000, 1001, 74176000},
    {"1280x720/15P",    3300, 750, 15,    1,    37125000}
};

//1920x1080 spec from smpte-296m-2001
static __TIMING_TBL_T _1920_1080_standard [] =
{
    {"1920x1080/120P",   2200, 1125, 120,   1,    297000000},
    {"1920x1080/119.88P",2200, 1125, 120000,1001, 296704000},
    {"1920x1080/100P",   2640, 1125, 100,   1,    297000000},
    {"1920x1080/60P",    2200, 1125, 60,    1,    148500000},
    {"1920x1080/59.94P", 2200, 1125, 60000, 1001, 148352000},
    {"1920x1080/50P",    2640, 1125, 50,    1,    148500000},
    {"1920x1080/48P",    2750, 1125, 48,    1,    148500000},
    {"1920x1080/47.95P", 2750, 1125, 48000, 1001, 148352000},
    {"1920x1080/30P",    2200, 1125, 30,    1,    74250000},
    {"1920x1080/29.97P", 2200, 1125, 30000, 1001, 74176000},
    {"1920x1080/25P",    2640, 1125, 25,    1,    74250000},
    {"1920x1080/24P",    2750, 1125, 24,    1,    74250000},
    {"1920x1080/23.97P", 2750, 1125, 24000, 1001, 74176000},
    {"1920x1080/12P",    2750, 1125, 12,    1,    37125000},
    {"1920x1080/15P",    2200, 1125, 15,    1,    37125000},
};


//3840x2160 spec from bt.2020
static __TIMING_TBL_T _3840_2160_standard [] =
{
    {"3840x2160/60P",    4400, 2250, 60,    1,    594000000},
    {"3840x2160/59.94P", 4400, 2250, 60000, 1001, 594000000},
    {"3840x2160/50P",    5280, 2250, 50,    1,    594000000},
    {"3840x2160/30P",    4400, 2250, 30,    1,    297000000},
    {"3840x2160/29.97P", 4400, 2250, 30000, 1001, 296704000},
    {"3840x2160/25P",    5280, 2250, 25,    1,    297000000},
    {"3840x2160/24P",    5500, 2250, 24,    1,    297000000},
    {"3840x2160/23.97P", 5500, 2250, 24000, 1001, 296704000},
    {"3840x2160/15P",    4400, 2250, 15   , 1,    148500000},
    {"3840x2160/14.98P", 4400, 2250, 15000, 1001, 148352000}
};


static __TIMING_MATRIX_T _standard_timing_matrix [] = 
{
    {720,  480,  _720_480_standard,   sizeof(_720_480_standard)/sizeof(_720_480_standard[0])},
    {720,  576,  _720_576_standard,   sizeof(_720_576_standard)/sizeof(_720_576_standard[0])},
    {1280, 720,  _1280_720_standard,  sizeof(_1280_720_standard)/sizeof(_1280_720_standard[0])},
    {1920, 1088, _1920_1080_standard, sizeof(_1920_1080_standard)/sizeof(_1920_1080_standard[0])},
    {3840, 2160, _3840_2160_standard, sizeof(_3840_2160_standard)/sizeof(_3840_2160_standard[0])},

    //end
    {0, 0, NULL, 0}
};


//interlace timing table
static __TIMING_TBL_T _720_480_standard_I [] =
{
    {"720x480/120I",   858,  525, 60,    1,    27027000},
    {"720x480/119.88I",858,  525, 60000, 1001, 27000000},
    {"720x480/60I",    858,  525, 30,    1,    27027000},       //use horizontal de ratio
    {"720x480/59.94I", 858,  525, 30000, 1001, 27000000}        //use horizontal de ratio 
};

static __TIMING_TBL_T _720_576_standard_I [] =
{
    {"720x576/100I",   864, 625, 50,    1,    27000000},
    {"720x576/50I",    864, 625, 25,    1,    27000000}        //use horizontal de ratio
};

static __TIMING_TBL_T _1920_1080_standard_I [] =
{
    {"1920x1080/100I",   2640, 1125, 50,    1,    148500000},
    {"1920x1080/60I",    2200, 1125, 30,    1,    74250000},
    {"1920x1080/59.94I", 2200, 1125, 30000, 1001, 74176000},
    {"1920x1080/50I",    2640, 1125, 25,    1,    74250000},
    {"1920x1080/24I",    2750, 1125, 24,    1,    74250000},
    {"1920x1080/23.97I", 2750, 1125, 24000, 1001, 74176000},
    {"1920x1080/12I",    2750, 1125, 12,    1,    37125000}
};

static __TIMING_MATRIX_T _standard_timing_matrix_I [] = 
{
    {720,  480,  _720_480_standard_I,   sizeof(_720_480_standard_I)/sizeof(_720_480_standard_I[0])},
    {720,  576,  _720_576_standard_I,   sizeof(_720_576_standard_I)/sizeof(_720_576_standard_I[0])},
    {1920, 1088, _1920_1080_standard_I, sizeof(_1920_1080_standard_I)/sizeof(_1920_1080_standard_I[0])},

    //end
    {0, 0, NULL, 0}
};

static __TIMING_TBL_T * _b2r_query_std_timing(__TIMING_MATRIX_T *ptMatrix, UINT16 u2HAct, UINT16 u2VAct, UINT32 u4Rate, UINT32 u4Scale)
{
    UINT32 i, j;
    __TIMING_TBL_T * ptRet = NULL;

    if (!ptMatrix)
    {
        return NULL;
    }

    for (i = 0; !ptRet && ptMatrix[i].u2MaxHAct != 0; i++)
    {
        __TIMING_MATRIX_T *pt = &ptMatrix[i];
        if ((u2HAct <= pt->u2MaxHAct) && (u2VAct <= pt->u2MaxVAct))
        {
            for (j = 0; j < pt->u2Size; j++)
            {
                if ((u4Rate ==  pt->ptTbl[j].u4Rate) && (u4Scale ==  pt->ptTbl[j].u4Scale))
                {
                    ptRet = &pt->ptTbl[j];
                    break;;
                }
            }
        }
    }

    return ptRet;
}

static __TIMING_TBL_T * _b2r_query_I_timing(UINT16 u2HAct, UINT16 u2VAct, UINT32 u4Rate, UINT32 u4Scale)
{
    return _b2r_query_std_timing(_standard_timing_matrix_I, u2HAct, u2VAct, u4Rate, u4Scale);
}

static __TIMING_TBL_T * _b2r_query_P_timing(UINT16 u2HAct, UINT16 u2VAct, UINT32 u4Rate, UINT32 u4Scale)
{
    return _b2r_query_std_timing(_standard_timing_matrix, u2HAct, u2VAct, u4Rate, u4Scale);
}



#endif /*_B2R_TIMING_H_*/
