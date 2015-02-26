/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMECCP160_H__
#define __DRMECCP160_H__


#define CRYPTO_ALIGN_4    __attribute__((aligned(4)))
#define CRYPTO_DWORD_ALIGN         CRYPTO_ALIGN_4



/* The following sizes are in BYTEs */
#define DRM_ECC160_PUBLIC_KEY_LEN   40
#define DRM_ECC160_PRIVATE_KEY_LEN  20
#define DRM_ECC160_PLAINTEXT_LEN    16
#define DRM_ECC160_CIPHERTEXT_LEN   80
#define DRM_ECC160_SIGNATURE_LEN    40
#define DRM_ECC160_SIGNATURE_B64LEN CCH_BASE64_EQUIV_SAL(DRM_ECC160_SIGNATURE_LEN)


#define PKSYMM_KEY_LEN_DRMV2        7
#define PKSYMM_ALG_TYPE_RC4         1

#if ( DRM_SUPPORT_WMDRMNET )
#define DRM_PKCRYPTO_CONTEXT_BUFFER_INTERNAL_SIZE ( 5500 + 4100 )
#else
#define DRM_PKCRYPTO_CONTEXT_BUFFER_INTERNAL_SIZE ( 5500 )
#endif

/* This is size for PK_DATA structure defined in pkcrypto file.
   This size of structure on x86 is 3036.
   I choose 3100 to allow larger size for different architecture
*/
#define DRM_PK_DATA_SIZE                          3200

#define LNGQDW 5
#define LNGQ DWORDS_TO_DIGITS(LNGQDW)
#define QBITS (LNGQDW * DWORD_BITS)

/*
** Length of ECC integer types in 32 bit values.
** For ECC 160 - lenght is 5 32 bit dwords.
** For ECC 256 - lenght is 8 32 bit dwords.
*/
#define LNGQ_MODULO_5  5
#define LNGQ_MODULO_8  8

#define DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE  ( DRM_PKCRYPTO_CONTEXT_BUFFER_INTERNAL_SIZE + sizeof( DRM_STACK_ALLOCATOR_CONTEXT ) + DRM_PK_DATA_SIZE + sizeof( CRYPTO_DWORD ) + sizeof( CRYPTO_VOID* ) ) 

typedef CRYPTO_DWORD_ALIGN struct __tagPUBKEY
{
    CRYPTO_BYTE y[ __CB_DECL(DRM_ECC160_PUBLIC_KEY_LEN) ];
} PUBKEY;

typedef CRYPTO_DWORD_ALIGN struct __tagPRIVKEY
{
    CRYPTO_BYTE x[ __CB_DECL(DRM_ECC160_PRIVATE_KEY_LEN) ];
} PRIVKEY;

typedef struct {
    PUBKEY pk;
    CRYPTO_BYTE version[__CB_DECL(VERSION_LEN)];  
} PKV;

/********************************************************************* 
** Structure for keeping of private key for signing of header. 
** The structure DRM_PRIVATE_KEY keeps ECC private key and
** enumeration value that corresponds to type of the key.
**   
**   privKey  -  The private key for signing of the header:
**   
**   keyType  - Type of the key
**   
**   Currently we have only one type of the key - ECC key.
**   In future new types will be added and new data for private keys
**********************************************************************/
typedef enum { DRM_PRIVATE_KEY_ECC } DRM_PRIVATE_KEY_TYPE;

typedef struct __tagDRM_PRIVATE_KEY

{ 
    DRM_PRIVATE_KEY_TYPE keyType;
    PRIVKEY              privKey;
} DRM_PRIVATE_KEY;


typedef struct {
    PKV pk;  /* pk.version indicates the pubkey needed to verify. */
    CRYPTO_BYTE sign[__CB_DECL(DRM_ECC160_SIGNATURE_LEN)];
} PKCERT;


/* */
/*  A set of old keys. */
/* */
typedef struct __tagDRMKEYPAIR
{
    PRIVKEY PrivKey;     /*  an old private key */
    PUBKEY  PubKey;      /*  an old public key */
} DRMKEYPAIR;


/*********************************************************************
**
**  Function:  CRYPTO_ECC_GenKeyPair_P160
**
**  Synopsis:  Generate a matched public and private key pair
**
**  Arguments:  
**     [f_pContext] -- Pointer to context the size of DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE
**     [f_ppubkey]  -- New public key
**     [f_pprivkey] -- New private key
**
*********************************************************************/
 CRYPTO_RESULT CRYPTO_ECC_GenKeyPair_P160( 
            CRYPTO_VOID *f_pContext,
            PUBKEY   *f_ppubkey,
            PRIVKEY  *f_pprivkey );

/*********************************************************************
**
**  Function:  CRYPTO_ECC_Encrypt_P160
**
**  Synopsis:  Encrypt a block of data with a given public key.
**
**  Arguments:  
**     [f_pContext] -- Pointer to context the size of DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE
**     [f_ppubkey]  -- Public key to encrypt with
**     [f_rgbIn]    -- Data to be encrypted
**     [f_rgbOut]   -- Buffer to hold the encrypted data value.
**
**  Notes:  Data is not encrpyted in place.  It is put in the rgbOut buffer.
*********************************************************************/
 CRYPTO_RESULT CRYPTO_ECC_Encrypt_P160( 
            CRYPTO_VOID *f_pContext, 
            const PUBKEY   *f_ppubkey,
            const CRYPTO_BYTE  f_rgbIn [__CB_DECL(DRM_ECC160_PLAINTEXT_LEN) ],
            CRYPTO_BYTE  f_rgbOut[__CB_DECL(DRM_ECC160_CIPHERTEXT_LEN)] );


/*********************************************************************
**
**  Function:  CRYPTO_ECC_Decrypt_P160
**
**  Synopsis:  
**
**  Arguments:  
**     [f_pContext] -- Pointer to context the size of DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE
**     [f_pprivkey] -- Private key to decrypt with
**     [f_rgbIn]    -- Encrypted bytes that are to be decrypted
**     [f_rgbOut]   -- Clear text result
**
**  Notes:  Data is not decrpyted in place.  It is put in the rgbOut buffer.
**
*********************************************************************/
 CRYPTO_RESULT CRYPTO_ECC_Decrypt_P160(
            CRYPTO_VOID *f_pContext,
            const PRIVKEY  *f_pprivkey,
            const CRYPTO_BYTE  f_rgbIn [__CB_DECL(DRM_ECC160_CIPHERTEXT_LEN)],
            CRYPTO_BYTE  f_rgbOut[__CB_DECL(DRM_ECC160_PLAINTEXT_LEN) ] );


/*********************************************************************
**
**  Function:  CRYPTO_ECC_Sign_P160
**
**  Synopsis:  Generate a digital signature with a private key
**
**  Arguments:  
**     [f_pContext]     -- Pointer to context the size of DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE
**     [f_privkey]      -- Private key to create a signature with
**     [f_pbData]       -- Array of bytes to create a signature over
**     [f_cbData]       -- Length of pbBuffer in bytes
**     [f_rgbSignature] -- Buffer to hold result signature
*********************************************************************/
 CRYPTO_RESULT CRYPTO_ECC_Sign_P160(
            CRYPTO_VOID  *f_pContext,
            const PRIVKEY   *f_privkey,
            const CRYPTO_BYTE  *f_pbData,
            CRYPTO_DWORD  f_cbData,
            CRYPTO_BYTE   f_rgbSignature[__CB_DECL(DRM_ECC160_SIGNATURE_LEN)] );

/*********************************************************************
**
**  Function:  CRYPTO_ECC_Verify_P160
**
**  Synopsis:  Verify a digital signature created by DRM_ECC_Sign_P160.
**
**  Arguments:  
**     [f_pContext]     -- Pointer to context the size of DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE
**     [f_ppubkey]      -- Pubkey to check the signature with
**     [f_pbData]       -- Data buffer that the signature was created over
**     [f_cbData]       -- Length of pbBuffer in bytes
**     [f_rgbSignature] -- The signature to verify
**
**  Returns:  TRUE if the signature verified correctly.  FALSE is it didn't
**
*********************************************************************/
CRYPTO_BOOL CRYPTO_ECC_Verify_P160( 
            CRYPTO_VOID  *f_pContext,
            const PUBKEY    *f_ppubkey, 
            const CRYPTO_BYTE  *f_pbData, 
            CRYPTO_DWORD  f_cbData, 
            const CRYPTO_BYTE   f_rgbSignature[__CB_DECL(DRM_ECC160_SIGNATURE_LEN)] );

/*********************************************************************
**
**  Function:  CRYPTO_ECC_ThreadUnSafePKInit, CRYPTO_ECC_ThreadSafeInit
**
**  Synopsis:  Initializes DRMBIGNUM_CONTEXT_STRUCT
**
**  Arguments:  
**     [f_pContext]     -- Pointer to context the size of DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE

**  Returns:  CRYPTO_SUCCESS if context is initialized.
**
*********************************************************************/
 CRYPTO_RESULT CRYPTO_ECC_ThreadUnSafePKInit( 
            CRYPTO_VOID* f_pContext,
            CRYPTO_VOID* f_pOEMContext );

CRYPTO_VOID CRYPTO_RSA_PKInit(
            CRYPTO_VOID *f_pContext );

CRYPTO_VOID CRYPTO_RSA_PKInitWithSize(
            CRYPTO_VOID *f_pContext,
            CRYPTO_DWORD f_dwSize );

 CRYPTO_RESULT CRYPTO_ECC_ThreadSafeInit( 
            CRYPTO_VOID* f_pContext,
            CRYPTO_VOID* f_pOEMContext );



#if CRYPTO_THREAD_SAFE
#define CRYPTO_ECC_PKInit( pContext, pOEMContext  ) CRYPTO_ECC_ThreadSafeInit( pContext, pOEMContext )
#else
#define CRYPTO_ECC_PKInit( pContext, pOEMContext  ) CRYPTO_ECC_ThreadUnSafePKInit( pContext, pOEMContext  )
#endif


#define TABLE_LAST 60
#define PK_DATA_HEAP_SIZE 250

typedef struct __tagDRM_STACK_ALLOCATOR_CONTEXT
{
    CRYPTO_DWORD nStackTop;    /* position of stack top */
    CRYPTO_DWORD cbStack;      /* size of stack */
    CRYPTO_BYTE *pbStack;      /* stack buffer */
    CRYPTO_BOOL  fWasPreAlloc; /* used to signal that we PreAlloc, and Alloc shouldn't fill mem with 0xaa */
} DRM_STACK_ALLOCATOR_CONTEXT;

typedef struct {
    digit_t q[LNGQ]; 
    digit_t a[LNGQ];
    digit_t b[LNGQ];
    digit_t r[LNGQ];         /* from singature file,   order of g */
    CRYPTO_DWORD   lngr;            /* Length of r (digits) */

    digit_t g[2*LNGQ];
    digit_t TABLE[ 2 * LNGQ * (TABLE_LAST+1) ];  /* TEST */

    mp_modulus_t   qmodulus;
    ecurve_t       ecurve;
    field_desc_t   fdesc;    
    reciprocal_1_t rrecip;             /* For division by r */
    
    DRM_STACK_ALLOCATOR_CONTEXT pkdHeap;
    CRYPTO_BYTE rgbHeap[__CB_DECL(PK_DATA_HEAP_SIZE)];
} PK_DATA;


typedef struct __DRMBIGNUM_CONTEXT_STRUCT
{
    DRM_STACK_ALLOCATOR_CONTEXT oHeap;
    CRYPTO_BYTE  rgbHeap[__CB_DECL(DRM_PKCRYPTO_CONTEXT_BUFFER_INTERNAL_SIZE)];
    PK_DATA   pkData;
    CRYPTO_DWORD fInited;
    CRYPTO_VOID *pOEMContext;
} DRMBIGNUM_CONTEXT_STRUCT;
        
#endif /* __DRMECCP160_H__ */
