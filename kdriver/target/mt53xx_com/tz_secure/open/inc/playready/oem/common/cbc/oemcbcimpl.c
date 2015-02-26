/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

/*
** chain-sum MAC scheme 4.5: multiply-&-swap rounds plus sum (reversible -- reversal code included)
*/

/*
** MP_C_STEP_P_COMBINED_LOOP should be defined
** before this file is #included.  See oemcbc.c for default implementations.
*/

#include <drmerr.h>
#include <oemcbc.h>
#include <drmbytemanip.h>
#include <drmlastinclude.h>

ENTER_PK_NAMESPACE_CODE;

/* pairwise independent function and summing step */
#define MP_C_STEP_P(pbData,L1, L2, L3, L4, L5, L6, t, sum) \
{\
    DRM_DWORD   dwTemp = 0;\
    BYTES_TO_DWORD( dwTemp, pbData );\
    pbData += __CB_DECL(SIZEOF(DRM_DWORD));\
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

#define MP_C_STEP_P_COMBINED(pbData, key, t, sum)                                         \
     MP_C_STEP_P( pbData, key->a1, key->b1, key->c1, key->d1, key->e1, key->f1, t, sum ); \
     MP_C_STEP_P( pbData, key->a2, key->b2, key->c2, key->d2, key->e2, key->f2, t, sum )

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

DRM_API DRM_DWORD DRM_CALL PR_DRM_MAC_inv32( DRM_DWORD n )
{
    unsigned long inv_approx = (3 * n) ^ 2;
    unsigned long error = 1 - n * inv_approx;

    inv_approx += inv_approx * error;
    error *= error;
    inv_approx += inv_approx * error;
    error *= error;

    return (inv_approx * error + inv_approx);
}

DRM_API DRM_VOID DRM_CALL PR_DRM_CBC64InvKey(
    __in const PR_DRM_CBCKey *cbckey,
    __out      PR_DRM_CBCKey *cbcInvKey )
{
    DRM_DWORD tempKey = 0;
    DRM_DWORD tempInv = 0;

    tempKey = cbckey->a1 * cbckey->a2;
    tempInv = PR_DRM_MAC_inv32( tempKey );
    cbcInvKey->a1 = tempInv * cbckey->a2;
    cbcInvKey->a2 = cbckey->a1 * tempInv;

    tempKey = cbckey->b1 * cbckey->b2;
    tempInv = PR_DRM_MAC_inv32( tempKey );
    cbcInvKey->b1 = tempInv * cbckey->b2;
    cbcInvKey->b2 = cbckey->b1 * tempInv;

    tempKey = cbckey->c1 * cbckey->c2;
    tempInv = PR_DRM_MAC_inv32( tempKey );
    cbcInvKey->c1 = tempInv * cbckey->c2;
    cbcInvKey->c2 = cbckey->c1 * tempInv;

    tempKey = cbckey->d1 * cbckey->d2;
    tempInv = PR_DRM_MAC_inv32( tempKey );
    cbcInvKey->d1 = tempInv * cbckey->d2;
    cbcInvKey->d2 = cbckey->d1 * tempInv;

    tempKey = cbckey->e1 * cbckey->e2;
    tempInv = PR_DRM_MAC_inv32( tempKey );
    cbcInvKey->e1 = tempInv * cbckey->e2;
    cbcInvKey->e2 = cbckey->e1 * tempInv;
}

DRM_API DRM_VOID DRM_CALL PR_DRM_CBC_Mac(
    __in   const DRM_BYTE      *pbData,
    __in         DRM_DWORD      cBlocks,
    __out        DRM_DWORD      rgdwKeys[2],
    __in   const PR_DRM_CBCKey    *pCBCkey )
{
    rgdwKeys[0] = rgdwKeys[1] = 0;
    while ( cBlocks > 0)
    {
        DRM_DWORD dw = 0;
        BYTES_TO_DWORD( dw, pbData );
        pbData += __CB_DECL(SIZEOF( DRM_DWORD ));
        MP_C_STEP(dw, pCBCkey->a1, pCBCkey->b1, pCBCkey->c1, pCBCkey->d1, pCBCkey->e1, pCBCkey->f1, rgdwKeys[1], rgdwKeys[0]);
        BYTES_TO_DWORD( dw, pbData );
        pbData += __CB_DECL(SIZEOF( DRM_DWORD ));
        MP_C_STEP(dw, pCBCkey->a2, pCBCkey->b2, pCBCkey->c2, pCBCkey->d2, pCBCkey->e2, pCBCkey->f2, rgdwKeys[1], rgdwKeys[0]);
        cBlocks -= 2;
    }
    return;
}

/*******************************************************************/

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

DRM_API DRM_VOID DRM_CALL PR_DRM_CBC_InverseMac(
    __inout_bcount( cBlocks * sizeof(DRM_DWORD) )        DRM_BYTE   *pbData,
    __in                                                 DRM_DWORD   cBlocks,
    __in                                           const PR_DRM_CBCKey *key,
    __in                                           const PR_DRM_CBCKey *ikey )
{
    DRM_DWORD tmp = 0,tmp2 = 0;
    DRM_DWORD sum64[2];

    /*
        Invert last two blocks (sum and 32-bit MAC).  This requires the encrypted last two
        blocks and the (dwNumBlocks-2) plaintext blocks.
    */
    PR_DRM_CBC_Mac(pbData, cBlocks - 2, sum64, key);
    BYTES_TO_DWORD( tmp, pbData + ((SIZEOF( DRM_DWORD)*(cBlocks - 1))/CB_NATIVE_BYTE));
    sum64[0] += tmp;

    /* last word */
    tmp -= key->f2;
    INV_STEP_C(ikey->a2, ikey->b2, ikey->c2, ikey->d2, ikey->e2);
    BYTES_TO_DWORD( tmp2, pbData + ((SIZEOF( DRM_DWORD)*(cBlocks - 2))/CB_NATIVE_BYTE));
    tmp -= ( tmp2 - sum64[0] );
    DWORD_TO_BYTES( pbData + ((SIZEOF( DRM_DWORD)*(cBlocks - 1)/CB_NATIVE_BYTE)), tmp );

    /* next-to-last word */
    tmp = (tmp2 - sum64[0]) - key->f1;
    INV_STEP_C(ikey->a1, ikey->b1, ikey->c1, ikey->d1, ikey->e1);
    tmp -= sum64[1];
    DWORD_TO_BYTES( pbData + ((SIZEOF( DRM_DWORD)*(cBlocks - 2)/CB_NATIVE_BYTE)), tmp );

    return;
}

DRM_API DRM_VOID DRM_CALL PR_DRM_CBC64InitState( __inout PR_DRM_CBCState *cbcstate )
{
    cbcstate->sum      = 0;
    cbcstate->t        = 0;
    cbcstate->dwBufLen = 0;
}

DRM_API DRM_VOID DRM_CALL PR_DRM_CBC64Init(
    __inout                                       PR_DRM_CBCKey   *cbckey,
    __inout                                       PR_DRM_CBCState *cbcstate,
    __in_bcount( SIZEOF( DRM_DWORD ) * 12 ) const DRM_BYTE     *pKey )
{
    const DRM_DWORD *p = NULL;

    cbcstate->sum = 0; cbcstate->t = 0; cbcstate->dwBufLen = 0;
    p = (const DRM_DWORD *)pKey;
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

DRM_API DRM_VOID DRM_CALL PR_DRM_CBC64Update(
    __in                  const PR_DRM_CBCKey    *key,
    __inout                     PR_DRM_CBCState  *cbcstate,
    __in                        DRM_DWORD      cbData,
    __in_bcount( cbData ) const DRM_BYTE      *pbData )
{
    DRM_DWORD       iData  = 0;
    DRM_DWORD       cbCopy = 0;
    DRM_DWORD       cbTemp = 0;
    const DRM_BYTE *pbTemp = NULL;

    if ( cbcstate->dwBufLen > 0 )
    {
        cbCopy = min( cbData, 8 - cbcstate->dwBufLen );

        for ( iData=0; iData < cbCopy; iData++ )
        {
            PUT_BYTE( cbcstate->buf,
                      cbcstate->dwBufLen + iData,
                      GET_BYTE(pbData, iData) );
        }

        cbcstate->dwBufLen += cbCopy;
        if ( cbcstate->dwBufLen == 8 )
        {
            pbTemp = cbcstate->buf;
            cbTemp = 1;
            MP_C_STEP_P_COMBINED_LOOP(cbcstate, pbTemp, cbTemp, key);
            cbcstate->dwBufLen = 0;
        }
    }

    cbTemp = (cbData - cbCopy) / 8;
    pbTemp = pbData + __CB_DECL(cbCopy);

    MP_C_STEP_P_COMBINED_LOOP(cbcstate, pbTemp, cbTemp, key);

    cbTemp = cbCopy + ((cbData-cbCopy) / 8) * 8;
    if ( cbTemp < cbData )
    {
        for ( iData=cbTemp; iData<cbData; iData++ )
        {
            PUT_BYTE( cbcstate->buf, iData - cbTemp, GET_BYTE( pbData, iData ) );
        }
        cbcstate->dwBufLen = cbData - cbTemp;
    }
}

DRM_API DRM_DWORD DRM_CALL PR_DRM_CBC64Finalize(
    __in            const PR_DRM_CBCKey   *key,
    __inout               PR_DRM_CBCState *cbcstate,
    __out_ecount(1)       DRM_DWORD    *pKey2 )
{
    DRM_DWORD       i       = 0;
    const DRM_BYTE *p       = NULL;
    DRM_DWORD       cbTemp  = 0;

    if ( cbcstate->dwBufLen > 0 )
    {
        for ( i=cbcstate->dwBufLen; i<8; i++ )
        {
            PUT_BYTE( cbcstate->buf, i, 0);
        }
        p = cbcstate->buf;
        cbTemp = 1;
        MP_C_STEP_P_COMBINED_LOOP(cbcstate, p, cbTemp, key);
        cbcstate->dwBufLen = 0;
    }

    *pKey2 = cbcstate->t;
    return cbcstate->sum;
}

DRM_API DRM_NO_INLINE DRM_DWORD DRM_CALL PR_DRM_CBC64Invert(
    __in            const PR_DRM_CBCKey  *key,
    __in            const PR_DRM_CBCKey  *ikey,
    __in                  DRM_DWORD    MacA1,
    __in                  DRM_DWORD    MacA2,
    __in                  DRM_DWORD    MacB1,
    __in                  DRM_DWORD    MacB2,
    __out_ecount(1)       DRM_DWORD   *pInvKey2 )
{
    DRM_DWORD tmp = 0;
    DRM_DWORD yn = 0, yn1 = 0, xn = 0, xn1 = 0;

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

    *pInvKey2 = (DRM_DWORD) xn1;
    return (DRM_DWORD) xn;
}

EXIT_PK_NAMESPACE_CODE;

