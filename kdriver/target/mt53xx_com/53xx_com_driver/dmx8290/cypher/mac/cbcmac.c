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
 * $RCSfile: dmx_es_api.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file rc4.c
 *  RC4 driver
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_drm_drvif.h"
#include "x_os.h"
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

/* pairwise independent function and summing step */
#define MP_C_STEP_P(pbData,L1, L2, L3, L4, L5, L6, t, sum) \
{\
    UINT32 dwTemp = 0;\
    BYTES_TO_DWORD( dwTemp, pbData );\
    pbData += (sizeof(UINT32));\
    t += dwTemp; \
    t *= L1; \
    t = WORDSWAP(t); \
    t *= L2; \
    t = WORDSWAP(t); \
    t *= L3; \
    t = WORDSWAP(t); \
    t *= L4; \
    t = WORDSWAP(t); \
    t *= L5; \
    t += L6; \
    sum += t; \
}


#define MP_C_STEP(Data,L1, L2, L3, L4, L5, L6, t, sum) \
    t += Data; \
    t *= L1; \
    t = WORDSWAP(t); \
    t *= L2; \
    t = WORDSWAP(t); \
    t *= L3; \
    t = WORDSWAP(t); \
    t *= L4; \
    t = WORDSWAP(t); \
    t *= L5; \
    t += L6; \
    sum += t;

/* step to reverse action of multiply-&-swap rounds */
#define INV_STEP_C(iL1, iL2, iL3, iL4, iL5) \
    tmp *= iL5; \
    tmp = WORDSWAP(tmp); \
    tmp *= iL4; \
    tmp = WORDSWAP(tmp); \
    tmp *= iL3; \
    tmp = WORDSWAP(tmp); \
    tmp *= iL2; \
    tmp = WORDSWAP(tmp); \
    tmp *= iL1;

                
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

UINT32 DRM_MAC_inv32(UINT32 n)
{
    INT32 x = n;
    INT32 i = 0;

    for (i = 0; i < 30; i++)
    {
        x *= x * n;
    }

    return (UINT32)x;
}

void DRM_CBC_Mac(
    UINT8      *pbData,
    UINT32      cBlocks,
    UINT32      rgdwKeys[2],
    const DRM_CBCKey    *pCBCkey )
{    
    rgdwKeys[0] = rgdwKeys[1] = 0;
    while ( cBlocks > 0)
    {
        UINT32 dw = 0;
        BYTES_TO_DWORD( dw, pbData );
        pbData += (sizeof( UINT32 ));
        MP_C_STEP(dw, pCBCkey->a1, pCBCkey->b1, pCBCkey->c1, pCBCkey->d1, pCBCkey->e1, pCBCkey->f1, rgdwKeys[1], rgdwKeys[0]);
        BYTES_TO_DWORD( dw, pbData );
        pbData += (sizeof( UINT32 ));
        MP_C_STEP(dw, pCBCkey->a2, pCBCkey->b2, pCBCkey->c2, pCBCkey->d2, pCBCkey->e2, pCBCkey->f2, rgdwKeys[1], rgdwKeys[0]);
        cBlocks -= 2;
    }
    return;
}

void DRM_CBC_InverseMac(
    UINT8   *pbData,
    UINT32   cBlocks,
    const DRM_CBCKey *key,
    const DRM_CBCKey *ikey )
{
    UINT32 tmp = 0,tmp2 = 0;
    UINT32 sum64[2];


    /* 
        Invert last two blocks (sum and 32-bit MAC).  This requires the encrypted last two
        blocks and the (dwNumBlocks-2) plaintext blocks. 
    */
    DRM_CBC_Mac(pbData, cBlocks - 2, sum64, key);
    BYTES_TO_DWORD( tmp, pbData + (sizeof(UINT32)*(cBlocks - 1)));
    sum64[0] += tmp;

    /* last word */    
    tmp -= key->f2;
    INV_STEP_C(ikey->a2, ikey->b2, ikey->c2, ikey->d2, ikey->e2);
    BYTES_TO_DWORD( tmp2, pbData + ((sizeof(UINT32)*(cBlocks - 2))));
    tmp -= ( tmp2 - sum64[0] );
    DWORD_TO_BYTES( pbData + ((sizeof(UINT32)*(cBlocks - 1))), tmp );

    /* next-to-last word */
    tmp = (tmp2 - sum64[0]) - key->f1;
    INV_STEP_C(ikey->a1, ikey->b1, ikey->c1, ikey->d1, ikey->e1);
    tmp -= sum64[1];
    DWORD_TO_BYTES( pbData + ((sizeof(UINT32)*(cBlocks - 2))), tmp );
        
    return;
}

void DRM_CBC64InitState( DRM_CBCState_T *cbcstate ) {
  cbcstate->sum = 0; cbcstate->t = 0; cbcstate->dwBufLen = 0;
}


void DRM_CBC64Init( DRM_CBCKey *cbckey, DRM_CBCState_T *cbcstate, UINT8 *pKey ) {
  UINT32 *p = NULL;

  cbcstate->sum = 0; cbcstate->t = 0; cbcstate->dwBufLen = 0;
  p = (UINT32 *)pKey;
  cbckey->a1 = *p++ | 0x00000001;
  cbckey->b1 = *p++ | 0x00000001;
  cbckey->c1 = *p++ | 0x00000001;
  cbckey->d1 = *p++ | 0x00000001;
  cbckey->e1 = *p++ | 0x00000001;
  cbckey->f1 = *p++ | 0x00000001;
  cbckey->a2 = *p++ | 0x00000001;
  cbckey->b2 = *p++ | 0x00000001;
  cbckey->c2 = *p++ | 0x00000001;
  cbckey->d2 = *p++ | 0x00000001;
  cbckey->e2 = *p++ | 0x00000001;
  cbckey->f2 = *p++ | 0x00000001;
}

void DRM_CBC64InvKey( DRM_CBCKey *cbckey, DRM_CBCKey *cbcInvKey ) {

  cbcInvKey->a1 = DRM_MAC_inv32( cbckey->a1 );
  cbcInvKey->a2 = DRM_MAC_inv32( cbckey->a2 );
  cbcInvKey->b1 = DRM_MAC_inv32( cbckey->b1 );
  cbcInvKey->b2 = DRM_MAC_inv32( cbckey->b2 );
  cbcInvKey->c1 = DRM_MAC_inv32( cbckey->c1 );
  cbcInvKey->c2 = DRM_MAC_inv32( cbckey->c2 );
  cbcInvKey->d1 = DRM_MAC_inv32( cbckey->d1 );
  cbcInvKey->d2 = DRM_MAC_inv32( cbckey->d2 );
  cbcInvKey->e1 = DRM_MAC_inv32( cbckey->e1 );
  cbcInvKey->e2 = DRM_MAC_inv32( cbckey->e2 );
  cbcInvKey->f1 = DRM_MAC_inv32( cbckey->f1 );
  cbcInvKey->f2 = DRM_MAC_inv32( cbckey->f2 );

}

void DRM_CBC64Update( 
    DRM_CBCKey  *key, 
    DRM_CBCState_T    *cbcstate,
    UINT32    cbData, 
    UINT8    *pbData )
{
    UINT32    iData  = 0;
    UINT32    cbCopy = 0;
    UINT32    cbTemp  = 0;
    UINT8    *pbTemp = NULL;
    
    if ( cbcstate->dwBufLen > 0 ) 
    {
        cbCopy = min( cbData, 8 - cbcstate->dwBufLen );
        
        for ( iData=0; iData < cbCopy; iData++ )
        {
    		if(cbcstate->dwBufLen + iData >= 8)
    		{
                // should not have this case
                return;
    		}
            cbcstate->buf[cbcstate->dwBufLen + iData] = pbData[iData];
        }
        
        cbcstate->dwBufLen += cbCopy;
        if ( cbcstate->dwBufLen == 8 ) 
        {
            pbTemp = cbcstate->buf;
            MP_C_STEP_P( pbTemp, key->a1, key->b1, key->c1, key->d1, key->e1, key->f1, cbcstate->t, cbcstate->sum );
            MP_C_STEP_P( pbTemp, key->a2, key->b2, key->c2, key->d2, key->e2, key->f2, cbcstate->t, cbcstate->sum );
            cbcstate->dwBufLen = 0;
        }
    }

    cbTemp = (cbData - cbCopy) / 8;
    pbTemp = pbData + cbCopy;

    while (cbTemp > 0) 
    {
        MP_C_STEP_P( pbTemp, key->a1, key->b1, key->c1, key->d1, key->e1, key->f1, cbcstate->t, cbcstate->sum );
        MP_C_STEP_P( pbTemp, key->a2, key->b2, key->c2, key->d2, key->e2, key->f2, cbcstate->t, cbcstate->sum );
        cbTemp--;
    }

    cbTemp = cbCopy + ((cbData-cbCopy) / 8) * 8;
    if ( cbTemp < cbData ) 
    {
        for ( iData=cbTemp; iData<cbData; iData++ )
        {
            cbcstate->buf[iData - cbTemp] = pbData[iData];
        }
        cbcstate->dwBufLen = cbData - cbTemp;
    }
}

UINT32 DRM_CBC64Finalize( DRM_CBCKey *key, DRM_CBCState_T *cbcstate, UINT32 *pKey2 ) {
    UINT32   i = 0;
    UINT8    *p = NULL;

    if ( cbcstate->dwBufLen > 0 ) 
    {
        for ( i=cbcstate->dwBufLen; i<8; i++ )
        {
            cbcstate->buf[i] = 0;
        }
        p = cbcstate->buf;
        MP_C_STEP_P( p, key->a1, key->b1, key->c1, key->d1, key->e1, key->f1, cbcstate->t, cbcstate->sum );
        MP_C_STEP_P( p, key->a2, key->b2, key->c2, key->d2, key->e2, key->f2, cbcstate->t, cbcstate->sum );
        cbcstate->dwBufLen = 0;
    }

    *pKey2 = cbcstate->t;
    return cbcstate->sum;
}



UINT32 DRM_CBC64Invert( DRM_CBCKey *key, DRM_CBCKey *ikey, UINT32 MacA1, UINT32 MacA2,
        UINT32 MacB1, UINT32 MacB2, UINT32 *pInvKey2)
{
    UINT32 tmp = 0;
    UINT32 yn = 0, yn1 = 0, xn = 0, xn1 = 0;

    MacA1 += MacB2;
    yn = MacB2;
    yn1 = MacB1 - MacA1;

    /* last word */
    tmp = yn - key->f2;
    INV_STEP_C(ikey->a2, ikey->b2, ikey->c2, ikey->d2, ikey->e2);
    xn = tmp - yn1;

    /* next-to-last word */
    tmp = yn1 - key->f1;
    INV_STEP_C(ikey->a1, ikey->b1, ikey->c1, ikey->d1, ikey->e1);
    xn1 = tmp - MacA2;

    *pInvKey2 = (UINT32) xn1;
    return (UINT32) xn;
}
