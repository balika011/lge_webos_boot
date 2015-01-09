/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: drv_mjc_dsr.h,v $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

#ifndef _DRV_MJC_DSR_H_
#define _DRV_MJC_DSR_H_

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/

#ifdef _C_MODEL_

#include "MJC.h"

#else

#endif


/*----------------------------------------------------------------------------*
 * Definitions
 *----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/

typedef struct mjc_dsr_sta
{
    UINT16  u2MVYBlkHistG1[7];
    UINT16  u2MVYBlkHistG2[7];
    UINT8   u1MVYRgnHistG1[11];
    UINT8   u1MVYRgnHistG2[11];

    INT16   i2PPCRgnMVX[9];
    INT8    i1PPCRgnMVY[9];
    INT16   i2PPCGlbMVX;
    INT8    i1PPCGlbMVY;
    UINT16 u2PPCGmvBlkCnt;

    INT8    i1ME2GlbMVY;
    INT8    i2ME2GlbMVX;

}MJC_DSR_STA_T;

typedef struct bnd_fg_sta
{
    UINT8   u1FmCntU[3];
    UINT8   u1FmCntD[3];

}BND_FG_STA_T;

/*----------------------------------------------------------------------------*
 * Function Members
 *----------------------------------------------------------------------------*/
#ifdef _C_MODEL_
void vDrvMJCDsrInit();
void vDrvMJCDsrGetSta();
void vDrvMJCDsrFrmStr();
#else
void vDrvMJCDsrInit(void);
void vDrvMJCDsrGetSta(void);
void vDrvMJCDsrFrmStr(void);
#endif

#endif //#ifndef _DRV_MJC_DSR_H_

