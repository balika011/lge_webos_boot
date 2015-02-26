/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMBCERTPARSER_H__
#define __DRMBCERTPARSER_H__
#include "stdlib.h"
#include "string.h"
#include "tzst.h"

#include "../crypto_types.h"
#include "drmbcert.h"


/*
** Minimal length of a certificate chain in bytes - 20
*/
#define DRM_MIN_CERT_CHAIN_LEN ( SIZEOF( CRYPTO_DWORD )   /* Header Constant */ \
                               + SIZEOF( CRYPTO_DWORD )   /* Version */ \
                               + SIZEOF( CRYPTO_DWORD )   /* Length */ \
                               + SIZEOF( CRYPTO_DWORD )   /* Flags */ \
                               + SIZEOF( CRYPTO_DWORD ) ) /* No of certificate entries: zero for minimal cert chain */

/*
** Defines for verification of object lengths in object headers
*/
#define DRM_BCERT_SIGNATURE_OBJECT_LEN ( SIZEOF( DRM_BCERT_OBJECT_HEADER )   /* Object header */ \
                                        + SIZEOF( CRYPTO_DWORD )      /* Signature type and length */ \
                                        + SIZEOF( SIGNATURE_P256 ) /* Signature data */ \
                                        + SIZEOF( CRYPTO_DWORD )      /* Issuer key length */ \
                                        + SIZEOF( PUBKEY_P256 ) )  /* Issuer key data */ 

#define DRM_BCERT_BASIC_INFO_OBJECT_LEN ( SIZEOF( DRM_BCERT_OBJECT_HEADER )  /* Object header */ \
                                        + DRM_BCERT_CERT_ID_LENGTH      /* Certificate ID */ \
                                        + SIZEOF( CRYPTO_DWORD )           /* Security level */ \
                                        + SIZEOF( CRYPTO_DWORD )           /* Certificate Flags */ \
                                        + SIZEOF( CRYPTO_DWORD )           /* Type */ \
                                        + SHA256_DIGEST_SIZE_IN_BYTES   /* Digest Value */ \
                                        + SIZEOF( CRYPTO_DWORD )           /* Expiration Date */ \
                                        + DRM_BCERT_CLIENT_ID_LENGTH )  /* Client ID */
                                        
#define DRM_BCERT_DEVICE_INFO_OBJECT_LEN ( SIZEOF( DRM_BCERT_OBJECT_HEADER ) /* Object header */ \
                                        + SIZEOF( CRYPTO_DWORD )         /* Max License Size */ \
                                        + SIZEOF( CRYPTO_DWORD )         /* Max Header Size */ \
                                        + SIZEOF( CRYPTO_DWORD ) )       /* Max License Chain Depth */

#define DRM_BCERT_FEATURES_OBJECT_MIN_LEN ( SIZEOF( DRM_BCERT_OBJECT_HEADER ) /* Object header */ \
                                          + SIZEOF( CRYPTO_DWORD ) )   /* Number of features: zero for minimal feature info */

#define DRM_BCERT_KEY_OBJECT_MIN_LEN ( SIZEOF( DRM_BCERT_OBJECT_HEADER ) /* Object header */ \
                                     + SIZEOF( CRYPTO_WORD )                /* Type */ \
                                     + SIZEOF( CRYPTO_WORD )                /* Length */ \
                                     + SIZEOF( CRYPTO_DWORD )               /* Flags */ \
                                     + ECC_P256_POINT_SIZE_IN_BYTES      /* Value */ \
                                     + SIZEOF( CRYPTO_DWORD )               /* Number of Entries */ \
                                     + SIZEOF( CRYPTO_DWORD ) )             /* One entry */
                                       
/*
** Minimal length of a certificate:
** Cert header, Basic Info, Feature Info with zero features,
** Key Info and Signature Info.
*/
#define DRM_MIN_CERTIFICATE_LEN ( SIZEOF( DRM_BCERT_HEADER )         \
                                + SIZEOF( DRM_BCERT_BASIC_INFO )     \
                                + DRM_BCERT_FEATURES_OBJECT_MIN_LEN  \
                                + DRM_BCERT_KEY_OBJECT_MIN_LEN       \
                                + DRM_BCERT_SIGNATURE_OBJECT_LEN )
                                

/*
** Minimal information to keep after a single certificate is parsed,
** which is needed verify the whole certificate chain.
*/
typedef struct __tagDRM_BCertMinimalData {
    DRM_BCERT_HEADER          oCertHeader;    
    DRM_BCERT_BASIC_INFO      oBasicInfo;
    DRM_BCERT_PUBKEY          oIssuerKey;
    DRM_BCERT_FEATURE_INFO    oFeatureInfo;
    DRM_BCERT_KEY_INFO        oKeyInfo;
    DRM_BCERT_SIGNATURE_INFO  oSignatureInfo;
} DRM_BCERT_MINIMALDATA;


/*
** Parsing error presentation, used in validation context structure.
*/
typedef struct {
    /* 
    ** Number of the cert in the chain, zero-based with leaf cert as 0 
    */
    CRYPTO_DWORD   cCertNumber;
    /* 
    ** Error code 
    */
    DRM_RESULT  dwResult;
} DRM_BCERT_VERIFICATIONRESULT;

#define DRM_MAX_VERIFICATIONRESULTS 10

/*
** Validation context: structure that controls certificate parser behavior.
** On exit it may contain a list of parsing errors.
*/
typedef struct {
    /* 
    ** [in] The latest expiration time of the cert chain - if 0 then don't verify it 
    */
    CRYPTOFILETIME        ftExpirationTime;
    /* 
    ** [in] Pointer to a public key of the cert chain root - if NULL then skip this verification 
    */
    const PUBKEY_P256  *pRootPubKey;
    /* 
    ** [in] Expected leaf certificate type - if DRM_BCERT_CERTTYPE_UNKNOWN then don't verify 
    */
    CRYPTO_DWORD          dwType;

    /* 
    ** [in] Set into TRUE if parser should verify cert signatures up the chain 
    */
    CRYPTO_BOOL           fVerifyCertSignatures;
    /* 
    ** [in] Set into FALSE if parser should ignore absense of extended data and don't verify signature over it
    */
    CRYPTO_BOOL           fDontFailOnMissingExtData;
    /* 
    ** [in] Array of key usages that must be found in a leaf certificate, 
    **      otherwise error out with DRM_E_BCERT_REQUIRED_KEYUSAGE_MISSING 
    */
    CRYPTO_DWORD          rgdwRequiredKeyUsages[DRM_BCERT_MAX_KEY_USAGES];
    /* 
    ** [in] Actual number of elements in rgdwRequiredKeyUsages 
    */
    CRYPTO_DWORD          cRequiredKeyUsages;
    /* 
    ** [in] If set into TRUE then parser will store non-fatal parser errors in array, keep parsing
    **      and return DRM_E_BCERT_VERIFICATION_ERRORS unless this is overriden by a fatal error
    **      (i.e. error after which the cert parsing cannot continue, for instance, length overflow)
    **      If set into FALSE then parsing stops immediately after any error.
    ** By default this field is FALSE.
    */
    CRYPTO_DWORD          fCollectErrorsInArrayAndContinue;    
    /* 
    ** [out] Array to store and return non-fatal verification errors.
    */
    DRM_BCERT_VERIFICATIONRESULT rgoResults[ DRM_MAX_VERIFICATIONRESULTS ];
    /* 
    ** [out] Actual number of members in rgoResults array, should be <= DRM_MAX_VERIFICATIONRESULTS 
    */
    CRYPTO_DWORD                    cResults;
    /* 
    ** [out] Number of a currently parsed certificate, zero-based with leaf cert as 0
    */
    CRYPTO_DWORD                    cCurrentCert;
} DRM_BCERT_VERIFICATIONCONTEXT;


DRM_RESULT DRM_BCert_InitVerificationContext(
    CRYPTOFILETIME                      *f_ftExpirationTime,
    const PUBKEY_P256                *f_pRootPubKey,
    const CRYPTO_DWORD                  f_dwCertType,
    CRYPTO_BOOL                         f_fVerifyCertSignatures,
    CRYPTO_BOOL                         f_fDontFailOnMissingExtData,
    CRYPTO_DWORD *f_pKeyUsages,
    CRYPTO_DWORD                        f_NumKeyUsages,
    CRYPTO_BOOL                         f_fCollectErrs,
    DRM_BCERT_VERIFICATIONCONTEXT    *f_pVerificationContext
);


DRM_RESULT DRM_BCert_ParseCertificateChain(
    const CRYPTO_BYTE                      *f_pbCertData,
    const CRYPTO_DWORD                     f_cbCertData,
    DRM_BCERT_VERIFICATIONCONTEXT *f_pVerificationContext );

DRM_RESULT DRM_BCert_GetChainHeader(
    const CRYPTO_BYTE         *f_pbCertData,
    const CRYPTO_DWORD        f_cbCertData,
    CRYPTO_DWORD              *f_bOffset,
    DRM_BCERT_CHAIN_HEADER *f_poChain );


DRM_RESULT DRM_BCert_GetCertificate(
    const CRYPTO_BYTE        *f_pbCertData,
    const CRYPTO_DWORD       f_cbCertData,
    CRYPTO_DWORD             *f_pbOffset,
    DRM_BCERT_CERTIFICATE *f_poCertificate,
    const CRYPTO_DWORD        f_dwRequiredCertType );


DRM_RESULT DRM_BCert_GetPublicKeyByUsage(
    const CRYPTO_BYTE    *f_pbCertData,
    const CRYPTO_DWORD    f_cbCertData,
    const CRYPTO_DWORD    f_dwCertIndex,
    const CRYPTO_DWORD    f_dwKeyUsage,
    PUBKEY_P256       *f_pPubkey,
    CRYPTO_DWORD         *f_pdwKeyUsages,
    CRYPTO_DWORD         *f_pcbKeyUsages );


DRM_RESULT DRM_BCert_GetPublicKey(
    const CRYPTO_BYTE      *f_pbCertData,
    const CRYPTO_DWORD     f_cbCertData,
    const CRYPTO_DWORD     f_dwCertIndex,
    PUBKEY_P256         *f_pPubkey );


DRM_RESULT DRM_BCert_FindObjectInCertByType(
    const CRYPTO_BYTE    *f_pbCertData,
    const CRYPTO_DWORD   f_cbCertData,
    CRYPTO_DWORD         *f_pbOffset,
    const CRYPTO_DWORD   f_dwObjectType );



#endif /* __DRMBCERTPARSER_H__ */
