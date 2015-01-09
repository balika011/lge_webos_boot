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
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: dmx_api.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file tz_api.c
 *  trustzone driver - main implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "tz.h"
#include "x_lint.h"
#include "x_debug.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define _TZ_IS_SUCCESS(rSmcArg)    (rSmcArg.u4Cmd == TZ_SUCCESS)?TRUE:FALSE

static void _TzSendSMC(TZ_SMC_ARG_T *prSmcArg)
{
#ifdef __KERNEL__
    asm volatile (
        ".arch_extension sec\n"
        "LDR    r0, =(0x545A3330)\n"
        "LDM    %1, {r1-r3}\n"
        "SMC    0\n"
        "MOV    %0, r0\n"
        : "=r" (prSmcArg->u4Cmd)
        : "r"  (prSmcArg)
        : "r0", "r1", "r2", "r3", "memory"
    );
#endif
}

BOOL _TZ_CTL(UINT32 u4Cmd, UINT32 u4PAddr, UINT32 u4Size)
{
    TZ_SMC_ARG_T rSmcArg;

    rSmcArg.u4Cmd   = u4Cmd;
    rSmcArg.u4Addr  = u4PAddr;
    rSmcArg.u4Size  = u4Size;

    _TzSendSMC(&rSmcArg);

    return _TZ_IS_SUCCESS(rSmcArg);
}

