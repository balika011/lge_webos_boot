/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMBCERT_H__
#define __DRMBCERT_H__

#include "../drmeccp256.h"
#include "../drmint64.h"

#define BYTES_PER_DWORD      SIZEOF( CRYPTO_DWORD )
#define PAD_AMOUNT(x)        ( ( (x) > 0 ) ? ( ( BYTES_PER_DWORD - ( (x) % BYTES_PER_DWORD ) ) % BYTES_PER_DWORD ) : 0 )

#define DRM_BCERT_OBJFLAG_EMPTY            0x0000
#define DRM_BCERT_OBJFLAG_MUST_UNDERSTAND  0x0001
#define DRM_BCERT_OBJFLAG_CONTAINER_OBJ    0x0002
 
#define DRM_BCERT_OBJTYPE_BASIC            0x0001
#define DRM_BCERT_OBJTYPE_DOMAIN           0x0002
#define DRM_BCERT_OBJTYPE_PC               0x0003
#define DRM_BCERT_OBJTYPE_DEVICE           0x0004
#define DRM_BCERT_OBJTYPE_FEATURE          0x0005
#define DRM_BCERT_OBJTYPE_KEY              0x0006
#define DRM_BCERT_OBJTYPE_MANUFACTURER     0x0007
#define DRM_BCERT_OBJTYPE_SIGNATURE        0x0008
#define DRM_BCERT_OBJTYPE_SILVERLIGHT      0x0009
#define DRM_BCERT_OBJTYPE_METERING         0x000a
#define DRM_BCERT_OBJTYPE_EXTDATASIGNKEY   0x000b
#define DRM_BCERT_OBJTYPE_EXTDATACONTAINER 0x000c
#define DRM_BCERT_OBJTYPE_EXTDATASIGNATURE 0x000d
#define DRM_BCERT_OBJTYPE_EXTDATA_HWID     0x000e
#define DRM_BCERT_OBJTYPE_SERVER           0x000f

#define DRM_BCERT_OBJTYPE_MAX_VALUE        0x000f

typedef struct _tagDrmBCertObjectHeader
{
    CRYPTO_WORD    wFlags;       /* bit field */
    CRYPTO_WORD    wType; 
    CRYPTO_DWORD   cbLength;
} DRM_BCERT_OBJECT_HEADER;


#define DRM_BCERT_MAX_CERTS_PER_CHAIN   6


/* ------------------------------------------------------------------------- */
/*  CERTIFICATE HEADER  */

#define DRM_BCERT_HEADER_TAG     0x43455254  /* "CERT" */
#define DRM_BCERT_VERSION        0x00000001

typedef struct _tagDrmBCertHeader
{
    CRYPTO_DWORD              dwHeaderTag;
    CRYPTO_DWORD              dwVersion;
    CRYPTO_DWORD              cbCertificate;
    CRYPTO_DWORD              cbCertificateSigned;
} DRM_BCERT_HEADER;


/* ------------------------------------------------------------------------- */
/*  BASIC INFO  */


#define DRM_BCERT_CERT_ID_LENGTH  16  /* 128 bits */
typedef struct _tagDrmBCertCertId
{
    CRYPTO_BYTE    rgb[ __CB_DECL( DRM_BCERT_CERT_ID_LENGTH ) ];
} DRM_BCERT_CERT_ID;

#define DRM_BCERT_SECURITYLEVEL_150    150  /* for testing */
#define DRM_BCERT_SECURITYLEVEL_2000   2000

#define DRM_BCERT_FLAGS_EMPTY             0x00000000 /* default */
#define DRM_BCERT_FLAGS_EXTDATA_PRESENT   0x00000001 /* presence of extended data */

#define DRM_BCERT_CERTTYPE_UNKNOWN       0x00000000
#define DRM_BCERT_CERTTYPE_PC            0x00000001
#define DRM_BCERT_CERTTYPE_DEVICE        0x00000002
#define DRM_BCERT_CERTTYPE_DOMAIN        0x00000003
#define DRM_BCERT_CERTTYPE_ISSUER        0x00000004
#define DRM_BCERT_CERTTYPE_CRL_SIGNER    0x00000005
#define DRM_BCERT_CERTTYPE_SERVICE       0x00000006
#define DRM_BCERT_CERTTYPE_SILVERLIGHT   0x00000007
#define DRM_BCERT_CERTTYPE_APPLICATION   0x00000008
#define DRM_BCERT_CERTTYPE_METERING      0x00000009
#define DRM_BCERT_CERTTYPE_KEYFILESIGNER 0x0000000a
#define DRM_BCERT_CERTTYPE_SERVER        0x0000000b

#define DRM_BCERT_CERTTYPE_MAX_VALUE     0x0000000b

#define DRM_BCERT_DEFAULT_EXPIRATION_DATE  0xFFFFFFFF

#define DRM_BCERT_CLIENT_ID_LENGTH  16  /* 128 bits */
typedef struct _tagDrmBCertClientId
{
    CRYPTO_BYTE    rgb[ __CB_DECL( DRM_BCERT_CLIENT_ID_LENGTH ) ];
} DRM_BCERT_CLIENT_ID;

typedef struct __tagSHA256_Digest
{
    CRYPTO_BYTE m_rgbDigest[ __CB_DECL( SHA256_DIGEST_SIZE_IN_BYTES ) ];
} DRM_SHA256_Digest;

typedef struct _tagDrmBCertBasicInfo
{
    DRM_BCERT_OBJECT_HEADER  Header;
    DRM_BCERT_CERT_ID        CertificateID;
    CRYPTO_DWORD             dwSecurityLevel;
    CRYPTO_DWORD             dwFlags;           /* bit field */
    CRYPTO_DWORD             dwType;
    DRM_SHA256_Digest        DigestValue;       /* hash of the public key value */
    CRYPTO_DWORD             dwExpirationDate;
    DRM_BCERT_CLIENT_ID      ClientID;
} DRM_BCERT_BASIC_INFO;


/* ------------------------------------------------------------------------- */
/*  DOMAIN INFO  */

typedef struct __tagDRM_GUID {
    CRYPTO_DWORD Data1;
    CRYPTO_WORD  Data2;
    CRYPTO_WORD  Data3;
    CRYPTO_BYTE  Data4 [__CB_DECL(8)];
} DRM_GUID;

#define DRM_BCERT_ACCOUNT_ID_LENGTH  SIZEOF(DRM_GUID)  /* 128 bits */
typedef struct _tagDrmBCertAccountId
{
    CRYPTO_BYTE    rgb[ __CB_DECL( DRM_BCERT_ACCOUNT_ID_LENGTH ) ];
} DRM_BCERT_ACCOUNT_ID;

#define DRM_BCERT_SERVICE_ID_LENGTH  16  /* 128 bits */
typedef struct _tagDrmBCertServiceId
{
    CRYPTO_BYTE    rgb[ __CB_DECL( DRM_BCERT_SERVICE_ID_LENGTH ) ];
} DRM_BCERT_SERVICE_ID;

#define DRM_BCERT_MAX_DOMAIN_URL_LENGTH  4096 
typedef struct _tagDrmBCertDomainURL
{
    CRYPTO_DWORD   cb;  /* URL length */
    CRYPTO_BYTE    rgb[ __CB_DECL( DRM_BCERT_MAX_DOMAIN_URL_LENGTH ) ];
} DRM_BCERT_DOMAIN_URL;

typedef struct _tagDrmBCertDomainInfo
{
    DRM_BCERT_OBJECT_HEADER  Header;
    DRM_BCERT_SERVICE_ID     ServiceID;
    DRM_BCERT_ACCOUNT_ID     AccountID;
    CRYPTO_DWORD             dwRevision;  /* a timestamp */
    DRM_BCERT_DOMAIN_URL     DomainURL;
} DRM_BCERT_DOMAIN_INFO;


/* ------------------------------------------------------------------------- */
/*  PC INFO  */


typedef struct _tagDrmBCertPCInfo
{
    DRM_BCERT_OBJECT_HEADER     Header;
    CRYPTO_DWORD                dwSecurityVersion;  /* A.B.C.D */
} DRM_BCERT_PC_INFO;

/* ------------------------------------------------------------------------- */
/*  SILVERLIGHT INFO  */

typedef struct
{
    DRM_BCERT_OBJECT_HEADER     Header;
    CRYPTO_DWORD                dwSecurityVersion;  /* A.B.C.D */
    CRYPTO_DWORD                dwPlatformIdentifier;
} DRM_BCERT_SILVERLIGHT_INFO;

/*  Platform Identifiers for the SilverLight INFO structure */
#define DRM_BCERT_SILVERLIGHT_PLATFORM_WINDOWS_X86  0
#define DRM_BCERT_SILVERLIGHT_PLATFORM_OSX_X86      1
#define DRM_BCERT_SILVERLIGHT_PLATFORM_MAX          1

/* ------------------------------------------------------------------------- */
/*  DEVICE INFO  */

#define DRM_BCERT_MAX_LICENSE_SIZE         10240  /* 10K */
#define DRM_BCERT_MAX_HEADER_SIZE          15360  /* 15K */
#define DRM_BCERT_MAX_LICENSE_CHAIN_DEPTH      2

typedef struct _tagDrmBCertDeviceInfo
{
    DRM_BCERT_OBJECT_HEADER     Header;
    CRYPTO_DWORD                cbMaxLicense;
    CRYPTO_DWORD                cbMaxHeader;
    CRYPTO_DWORD                dwMaxChainDepth;
} DRM_BCERT_DEVICE_INFO;


/* ------------------------------------------------------------------------- */
/*  METERING INFO  */

#define DRM_BCERT_METERING_ID_LENGTH  SIZEOF(DRM_GUID)  /* 128 bits */
typedef struct _tagDrmBCertMeteringId
{
    CRYPTO_BYTE    rgb[ __CB_DECL( DRM_BCERT_METERING_ID_LENGTH ) ];
} DRM_BCERT_METERING_ID;

#define DRM_BCERT_MAX_METERING_URL_LENGTH  4096 
typedef struct _tagDrmBCertMeteringURL
{
    CRYPTO_DWORD   cb;  /* URL length */
    CRYPTO_BYTE    rgb[ __CB_DECL( DRM_BCERT_MAX_METERING_URL_LENGTH ) ];
} DRM_BCERT_METERING_URL;

typedef struct
{
    DRM_BCERT_OBJECT_HEADER  Header;
    DRM_BCERT_METERING_ID    MeteringID;
    DRM_BCERT_METERING_URL   MeteringURL;
} DRM_BCERT_METERING_INFO;

/* ------------------------------------------------------------------------- */
/*  SERVER INFO  */

typedef struct _tagDrmBCertServerInfo
{
    DRM_BCERT_OBJECT_HEADER     Header;
    CRYPTO_DWORD                dwWarningDays;  /* number of days before expiry to start warning */
} DRM_BCERT_SERVER_INFO;


/* ------------------------------------------------------------------------- */
/*  FEATURE INFO  */

#define DRM_BCERT_MAX_FEATURES 32    /* array size with room to grow */

#define DRM_BCERT_FEATURE_TRANSMITTER             0x00000001
#define DRM_BCERT_FEATURE_RECEIVER                0x00000002
#define DRM_BCERT_FEATURE_SHARED_CERTIFICATE      0x00000003
#define DRM_BCERT_FEATURE_SECURE_CLOCK            0x00000004
#define DRM_BCERT_FEATURE_ANTIROLLBACK_CLOCK      0x00000005
/*
** The next three feature flags should not be explicitly set in the certs
** because for any cert their values are already known 
** (i.e. every device must support metering, license sync and symmetric optimization)
** These features will stay deprecated as long as they are not optional.
*/
#define DRM_BCERT_FEATURE_RESERVED_METERING       0x00000006
#define DRM_BCERT_FEATURE_RESERVED_LICSYNC        0x00000007
#define DRM_BCERT_FEATURE_RESERVED_SYMOPT         0x00000008
#define DRM_BCERT_FEATURE_SUPPORTS_CRLS           0x00000009

#define DRM_BCERT_FEATURE_MAX_VALUE               0x00000009


#define BCERT_FEATURE_BIT(dwFeature)     (1 << ((dwFeature)-1))


typedef struct _tagDrmBCertFeatureInfo
{
    DRM_BCERT_OBJECT_HEADER     Header;
    CRYPTO_DWORD                dwNumFeatureEntries;  /* this can be zero */
    CRYPTO_DWORD                rgdwFeatureSet[ DRM_BCERT_MAX_FEATURES ];
} DRM_BCERT_FEATURE_INFO;


/* ------------------------------------------------------------------------- */
/*  KEY INFO  */


/* List of supported key types. (currently just one) */
#define DRM_BCERT_KEYTYPE_ECC256   0x0001

#define DRM_BCERT_MAX_KEY_USAGES 32  /* array size with room to grow */

#define DRM_BCERT_KEYUSAGE_UNKNOWN             0x00000000                
#define DRM_BCERT_KEYUSAGE_SIGN                0x00000001
#define DRM_BCERT_KEYUSAGE_ENCRYPT_KEY         0x00000002
#define DRM_BCERT_KEYUSAGE_SIGN_CRL            0x00000003
#define DRM_BCERT_KEYUSAGE_ISSUER_ALL          0x00000004
#define DRM_BCERT_KEYUSAGE_ISSUER_INDIV        0x00000005
#define DRM_BCERT_KEYUSAGE_ISSUER_DEVICE       0x00000006
#define DRM_BCERT_KEYUSAGE_ISSUER_LINK         0x00000007
#define DRM_BCERT_KEYUSAGE_ISSUER_DOMAIN       0x00000008
#define DRM_BCERT_KEYUSAGE_ISSUER_SILVERLIGHT  0x00000009
#define DRM_BCERT_KEYUSAGE_ISSUER_APPLICATION  0x0000000a
#define DRM_BCERT_KEYUSAGE_ISSUER_CRL          0x0000000b
#define DRM_BCERT_KEYUSAGE_ISSUER_METERING     0x0000000c
#define DRM_BCERT_KEYUSAGE_ISSUER_SIGN_KEYFILE 0x0000000d
#define DRM_BCERT_KEYUSAGE_SIGN_KEYFILE        0x0000000e
#define DRM_BCERT_KEYUSAGE_ISSUER_SERVER       0x0000000f

#define DRM_BCERT_KEYUSAGE_MAX_VALUE           0x0000000f


#define BCERT_KEYUSAGE_BIT(dwKeyUsage)     (1 << ((dwKeyUsage)-1))


/*
** The following byte value should be equal to the largest public key length supported
*/
#define DRM_BCERT_MAX_PUBKEY_VALUE_LENGTH     SIZEOF( PUBKEY_P256 )

typedef struct _tagDrmBCertPubKey
{
    CRYPTO_BYTE    rgb[ __CB_DECL( DRM_BCERT_MAX_PUBKEY_VALUE_LENGTH ) ];
} DRM_BCERT_PUBKEY;


typedef struct _tagDrmBCertCertKey
{
    CRYPTO_WORD                 wType;
    CRYPTO_WORD                 wLength;           /* in bits */
    CRYPTO_DWORD                dwFlags;           /* bit field; should be ignored */
    DRM_BCERT_PUBKEY            Value;
    CRYPTO_DWORD                dwNumUsageEntries; /* this cannot be zero */
    CRYPTO_DWORD                dwUsageSet[ DRM_BCERT_MAX_KEY_USAGES ];
} DRM_BCERT_CERTKEY;


typedef struct _tagDrmBCertKeyInfo
{
    DRM_BCERT_OBJECT_HEADER     Header;
    CRYPTO_DWORD                dwNumKeys;                             /* actual number of items in array, > 0 */
    DRM_BCERT_CERTKEY           rgoKeys[ DRM_BCERT_MAX_KEY_USAGES ];   /* array of public keys, up to number of various key usages */
}  DRM_BCERT_KEY_INFO;


/* ------------------------------------------------------------------------- */
/*  MANUFACTURER INFO  */

#define DRM_BCERT_MAX_MANUFACTURER_STRING_LENGTH  128   /* bytes */
typedef struct _tagDrmBCertManufacturerString
{
    CRYPTO_DWORD   cb;
    CRYPTO_BYTE    rgb[ __CB_DECL( DRM_BCERT_MAX_MANUFACTURER_STRING_LENGTH ) ];
} DRM_BCERT_MANUFACTURER_STRING;

typedef struct _tagDrmBCertManufacturerStrings
{
    DRM_BCERT_MANUFACTURER_STRING  ManufacturerName; 
    DRM_BCERT_MANUFACTURER_STRING  ModelName;         
    DRM_BCERT_MANUFACTURER_STRING  ModelNumber;     
} DRM_BCERT_MANUFACTURER_STRINGS;

typedef struct _tagDrmBCertManufacturerInfo
{
    DRM_BCERT_OBJECT_HEADER             Header;
    CRYPTO_DWORD                        dwFlags; /* bit field; should be ignored */
    DRM_BCERT_MANUFACTURER_STRINGS      ManufacturerStrings;
} DRM_BCERT_MANUFACTURER_INFO;


/* ------------------------------------------------------------------------- */
/*  EXTENDED DATA SIGNATURE KEY INFO */

typedef struct _tagDrmBCertExtDataSignKeyInfo
{
    DRM_BCERT_OBJECT_HEADER        Header;
    CRYPTO_WORD                    wType; 
    CRYPTO_WORD                    wLength;           /* in bits */
    CRYPTO_DWORD                   dwFlags;           /* bit field; should be ignored */
    DRM_BCERT_PUBKEY               Value;
} DRM_BCERT_EXTDATA_SIGNKEY_INFO;


/* ------------------------------------------------------------------------- */
/*  SIGNATURE INFO  */

#define DRM_BCERT_SIGNATURE_TYPE_P256         0x0001    /* ECDSA-P-256-SHA-256 */

/*
** The following byte value should be equal to the largest signature length supported
*/
#define DRM_BCERT_MAX_SIGNATURE_DATA_LENGTH   SIZEOF( SIGNATURE_P256 )

typedef struct _tagDrmBCertSignatureData
{
    CRYPTO_WORD    cb;   /* bytes */
    CRYPTO_BYTE    rgb[ __CB_DECL( DRM_BCERT_MAX_SIGNATURE_DATA_LENGTH ) ];
} DRM_BCERT_SIGNATURE_DATA;

typedef struct _tagDrmBCertSignatureInfo
{
    DRM_BCERT_OBJECT_HEADER            Header;
    CRYPTO_WORD                        wSignatureType;
    DRM_BCERT_SIGNATURE_DATA           SignatureData;
    CRYPTO_DWORD                       dwIssuerKeyLength;
    DRM_BCERT_PUBKEY                   IssuerKey;         /* = public key in next cert */
} DRM_BCERT_SIGNATURE_INFO;


/* ------------------------------------------------------------------------- */
/*  EXTENDED DATA RECORD */

typedef struct _tagBCertExtDataRecord
{
    DRM_BCERT_OBJECT_HEADER        Header;
    CRYPTO_DWORD                   dwDataLength;
    CRYPTO_BYTE                    *pbData;    /* this is a pointer, not a buffer */
} DRM_BCERT_EXTDATA_RECORD;

/* ------------------------------------------------------------------------- */
/*  EXTENDED DATA SIGNATURE */

typedef struct _tagDrmBCertExtDataSignature
{
    DRM_BCERT_OBJECT_HEADER        Header;
    CRYPTO_WORD                    wSignatureType; 
    DRM_BCERT_SIGNATURE_DATA       SignatureData;
} DRM_BCERT_EXTDATA_SIGNATURE;


/* ------------------------------------------------------------------------- */
/*  EXTENDED DATA CONTAINER */


#define DRM_BCERT_MAX_EXTDATA_RECORDS           1

typedef struct _tagBCertExtDataContainer
{
    DRM_BCERT_OBJECT_HEADER        Header;
    CRYPTO_DWORD                   cbRecordsCount;
    DRM_BCERT_EXTDATA_RECORD       Records[DRM_BCERT_MAX_EXTDATA_RECORDS];        
    DRM_BCERT_EXTDATA_SIGNATURE    Signature;
} DRM_BCERT_EXTDATA_CONTAINER;


/* ------------------------------------------------------------------------- */
/*  CERTIFICATE STRUCTURE (for reference) */


typedef struct _tagDrmBCertCertificate
{
    DRM_BCERT_HEADER              Header;
    DRM_BCERT_BASIC_INFO          BasicInfo;

    union _tagBCertType
    {
        DRM_BCERT_DOMAIN_INFO       DomainInfo;
        DRM_BCERT_PC_INFO           PCInfo;
        DRM_BCERT_DEVICE_INFO       DeviceInfo;
        DRM_BCERT_SILVERLIGHT_INFO  SilverLightInfo;
        DRM_BCERT_METERING_INFO     MeteringInfo;
        DRM_BCERT_SERVER_INFO       ServerInfo;
    } Type; /* optional */

    DRM_BCERT_FEATURE_INFO        FeatureInfo;
    DRM_BCERT_KEY_INFO            KeyInfo;
    DRM_BCERT_MANUFACTURER_INFO   ManufacturerInfo;  /* optional */

    DRM_BCERT_EXTDATA_SIGNKEY_INFO ExtDataSignatureInfo;
    /* New elements defined post-v1 go here */
    
    DRM_BCERT_SIGNATURE_INFO      SignatureInfo;

    DRM_BCERT_EXTDATA_CONTAINER   ExtDataContainer;
} DRM_BCERT_CERTIFICATE;


/* ------------------------------------------------------------------------- */
/*  CHAIN HEADER  */

#define DRM_BCERT_CHAIN_HEADER_TAG     0x43484149  /* "CHAI" */
#define DRM_BCERT_CHAIN_VERSION        0x00000001

typedef struct _tagDrmBCertChainHeader
{
    CRYPTO_DWORD              dwHeaderTag; /* = DRM_BCERT_CHAIN_HEADER_ID */
    CRYPTO_DWORD              dwVersion;   /* = DRM_BCERT_CHAIN_VERSION   */
    CRYPTO_DWORD              cbChain;
    CRYPTO_DWORD              dwFlags;     /* bit field; should be ignored */
    CRYPTO_DWORD              cCerts;
} DRM_BCERT_CHAIN_HEADER;


/*
** The following two offsets are relative to the beginning of the certificate chain
*/
#define DRM_BCERT_CHAIN_SIZE_OFFSET     ( 2 * SIZEOF( CRYPTO_DWORD ) )
#define DRM_BCERT_CHAIN_COUNT_OFFSET    ( 4 * SIZEOF( CRYPTO_DWORD ) )



typedef UINT32  DRM_RESULT;
#define DRM_SUCCESS                             ((DRM_RESULT)0x00000000L)
#define DRM_S_FALSE                             ((DRM_RESULT)0x00000001L)
#define DRM_SUCCEEDED(Status) ((DRM_RESULT)(Status) >= 0)
#define DRM_UI64LITERAL(a,b) { b, a  }

#define C_TICS_PER_SECOND 10000000
#define C_SECONDS_FROM_1601_TO_1970  CRYPTO_UI64Mul(CRYPTO_UI64(1164447360), CRYPTO_UI64(10))

#define CREATE_FILE_TIME(dw, ft) {  \
            CRYPTO_UINT64 ui64Tics = CRYPTO_UI64Add( CRYPTO_UI64( dw ), C_SECONDS_FROM_1601_TO_1970 ); \
            ui64Tics = CRYPTO_UI64Mul( ui64Tics, CRYPTO_UI64( C_TICS_PER_SECOND ) );                \
            UI64_TO_FILETIME( ui64Tics, ft );                                                 \
        }


#define ChkBoundsLT( index, arrSize )               \
do                                                  \
{                                                   \
    if ( index >= arrSize )                         \
    {                                               \
        ChkDR( DRM_E_BUFFER_BOUNDS_EXCEEDED );      \
    }                                               \
}                                                   \
while ( FALSE )

/*
 * MessageId: DRM_E_FAIL
 *
 * MessageText:
 *
 * The requested operation failed.
 *
 */
#define DRM_E_FAIL                       ((DRM_RESULT)0x80004005L)


/*
 * MessageId: DRM_E_UTF_UNEXPECTED_END
 *
 * MessageText:
 *
 * Unexpected end of data in the middle of multibyte character.
 *
 */
#define DRM_E_UTF_UNEXPECTED_END         ((DRM_RESULT)0x8004CA00L)


/*
 * MessageId: DRM_E_UTF_INVALID_CODE
 *
 * MessageText:
 *
 * UTF character maps into a code with invalid value.
 *
 */
#define DRM_E_UTF_INVALID_CODE           ((DRM_RESULT)0x8004CA01L)


/*
 * MessageId: DRM_E_BUFFER_BOUNDS_EXCEEDED
 *
 * MessageText:
 *
 * An array is being referenced outside of it's bounds.
 *
 */
#define DRM_E_BUFFER_BOUNDS_EXCEEDED     ((DRM_RESULT)0x8004C073L)

/*
 * MessageId: DRM_E_BCERT_STRING_NOT_NULL_TERMINATED
 *
 * MessageText:
 *
 * Data string is not null-teminated
 *
 */
#define DRM_E_BCERT_STRING_NOT_NULL_TERMINATED ((DRM_RESULT)0x8004C819L)

/*
 * MessageId: DRM_E_BCERT_INVALID_CERT_TYPE
 *
 * MessageText:
 *
 * An invalid certificate type was encountered
 *
 */
#define DRM_E_BCERT_INVALID_CERT_TYPE    ((DRM_RESULT)0x8004C802L)

/*
 * MessageId: DRM_E_BCERT_INVALID_KEY_USAGE
 *
 * MessageText:
 *
 * An invalid public key usage was encountered
 *
 */
#define DRM_E_BCERT_INVALID_KEY_USAGE    ((DRM_RESULT)0x8004C804L)

/*
 * MessageId: DRM_E_BCERT_INVALID_FEATURE
 *
 * MessageText:
 *
 * An invalid feature entry was encountered
 *
 */
#define DRM_E_BCERT_INVALID_FEATURE      ((DRM_RESULT)0x8004C803L)

/*
 * MessageId: DRM_E_BCERT_INVALID_CHAIN_LENGTH
 *
 * MessageText:
 *
 * Cert chain length value is invalid
 *
 */
#define DRM_E_BCERT_INVALID_CHAIN_LENGTH ((DRM_RESULT)0x8004C823L)

/*
 * MessageId: DRM_E_BCERT_INVALID_SECURITY_LEVEL
 *
 * MessageText:
 *
 * An invalid security level was encountered
 *
 */
#define DRM_E_BCERT_INVALID_SECURITY_LEVEL ((DRM_RESULT)0x8004C80BL)


/*
 * MessageId: DRM_E_BCERT_ISSUERKEY_KEYINFO_MISMATCH
 *
 * MessageText:
 *
 * The cert's Issuer Key does not match key info in the next cert
 *
 */
#define DRM_E_BCERT_ISSUERKEY_KEYINFO_MISMATCH ((DRM_RESULT)0x8004C81EL)

/*
 * MessageId: DRM_E_ARITHMETIC_OVERFLOW
 *
 * MessageText:
 *
 * Arithmetic result exceeded maximum value.
 *
 */
#define DRM_E_ARITHMETIC_OVERFLOW        ((DRM_RESULT)0x80070216L)

/*
 * MessageId: DRM_E_BUFFERTOOSMALL
 *
 * MessageText:
 *
 * The data area passed to a function is too small.
 *
 */
#define DRM_E_BUFFERTOOSMALL             ((DRM_RESULT)0x8007007AL)


/*
 * MessageId: DRM_E_BCERT_INVALID_CERT_HEADER_TAG
 *
 * MessageText:
 *
 * Cert header tag is invalid
 *
 */
#define DRM_E_BCERT_INVALID_CERT_HEADER_TAG ((DRM_RESULT)0x8004C824L)

/*
 * MessageId: DRM_E_BCERT_INVALID_CERT_VERSION
 *
 * MessageText:
 *
 * Cert version is invalid
 *
 */
#define DRM_E_BCERT_INVALID_CERT_VERSION ((DRM_RESULT)0x8004C825L)

/*
 * MessageId: DRM_E_BCERT_INVALID_CERT_LENGTH
 *
 * MessageText:
 *
 * Cert length value is invalid
 *
 */
#define DRM_E_BCERT_INVALID_CERT_LENGTH  ((DRM_RESULT)0x8004C826L)

/*
 * MessageId: DRM_E_BCERT_INVALID_SIGNEDCERT_LENGTH
 *
 * MessageText:
 *
 * Length of signed portion of certificate is invalid
 *
 */
#define DRM_E_BCERT_INVALID_SIGNEDCERT_LENGTH ((DRM_RESULT)0x8004C827L)

/*
 * MessageId: DRM_E_BCERT_EXTDATAFLAG_CERT_TYPE_MISMATCH
 *
 * MessageText:
 *
 * Certificate of this type cannot have extended data flag set
 *
 */
#define DRM_E_BCERT_EXTDATAFLAG_CERT_TYPE_MISMATCH ((DRM_RESULT)0x8004C832L)

/*
 * MessageId: DRM_E_BCERT_DOMAIN_URL_TOO_LONG
 *
 * MessageText:
 *
 * The domain URL contains too many ASCII characters
 *
 */
#define DRM_E_BCERT_DOMAIN_URL_TOO_LONG  ((DRM_RESULT)0x8004C812L)

/*
 * MessageId: DRM_E_BCERT_INVALID_WARNING_DAYS
 *
 * MessageText:
 *
 * An invalid server certificate expiration warning days. Warning days must be greater than zero.
 *
 */
#define DRM_E_BCERT_INVALID_WARNING_DAYS ((DRM_RESULT)0x8004C83CL)

/*
 * MessageId: DRM_E_BCERT_INVALID_SECURITY_VERSION
 *
 * MessageText:
 *
 * An invalid Indiv Box security version was encountered
 *
 */
#define DRM_E_BCERT_INVALID_SECURITY_VERSION ((DRM_RESULT)0x8004C805L)

/*
 * MessageId: DRM_E_BCERT_INVALID_PLATFORM_IDENTIFIER
 *
 * MessageText:
 *
 * An invalid Platform Identifier was specified
 *
 */
#define DRM_E_BCERT_INVALID_PLATFORM_IDENTIFIER ((DRM_RESULT)0x8004C828L)

/*
 * MessageId: DRM_E_BCERT_OBJECTHEADER_LEN_TOO_BIG
 *
 * MessageText:
 *
 * Object length in object header is too big
 *
 */
#define DRM_E_BCERT_OBJECTHEADER_LEN_TOO_BIG ((DRM_RESULT)0x8004C81AL)

/*
 * MessageId: DRM_E_BCERT_UNEXPECTED_OBJECT_HEADER
 *
 * MessageText:
 *
 * Object header has unexpected values
 *
 */
#define DRM_E_BCERT_UNEXPECTED_OBJECT_HEADER ((DRM_RESULT)0x8004C81DL)

/*
 * MessageId: DRM_E_BCERT_INVALID_MAX_LICENSE_SIZE
 *
 * MessageText:
 *
 * An invalid maximum license size value was encountered
 *
 */
#define DRM_E_BCERT_INVALID_MAX_LICENSE_SIZE ((DRM_RESULT)0x8004C808L)

/*
 * MessageId: DRM_E_BCERT_UNEXPECTED_OBJECT_HEADER
 *
 * MessageText:
 *
 * Object header has unexpected values
 *
 */
#define DRM_E_BCERT_UNEXPECTED_OBJECT_HEADER ((DRM_RESULT)0x8004C81DL)

/*
 * MessageId: DRM_E_BCERT_INVALID_KEY_TYPE
 *
 * MessageText:
 *
 * An invalid public key type was encountered
 *
 */
#define DRM_E_BCERT_INVALID_KEY_TYPE     ((DRM_RESULT)0x8004C806L)

/*
 * MessageId: DRM_E_BCERT_INVALID_MAX_FEATURES
 *
 * MessageText:
 *
 * Number of features is invalid
 *
 */
#define DRM_E_BCERT_INVALID_MAX_FEATURES ((DRM_RESULT)0x8004C820L)


/*
 * MessageId: DRM_E_BCERT_MANUFACTURER_STRING_TOO_LONG
 *
 * MessageText:
 *
 * The manufacturer string is too long
 *
 */
#define DRM_E_BCERT_MANUFACTURER_STRING_TOO_LONG ((DRM_RESULT)0x8004C839L)

/*
 * MessageId: DRM_E_BCERT_INVALID_KEY_LENGTH
 *
 * MessageText:
 *
 * An invalid public key length was encountered
 *
 */
#define DRM_E_BCERT_INVALID_KEY_LENGTH   ((DRM_RESULT)0x8004C807L)

/*
 * MessageId: DRM_E_BCERT_INVALID_SIGNATURE_TYPE
 *
 * MessageText:
 *
 * An invalid signature type was encountered
 *
 */
#define DRM_E_BCERT_INVALID_SIGNATURE_TYPE ((DRM_RESULT)0x8004C800L)

/*
 * MessageId: DRM_E_BCERT_INVALID_ISSUERKEY_LENGTH
 *
 * MessageText:
 *
 * IssuerKey Length value is invalid
 *
 */
#define DRM_E_BCERT_INVALID_ISSUERKEY_LENGTH ((DRM_RESULT)0x8004C81BL)

/*
 * MessageId: DRM_E_BCERT_INVALID_SIGNATURE_TYPE
 *
 * MessageText:
 *
 * An invalid signature type was encountered
 *
 */
#define DRM_E_BCERT_INVALID_SIGNATURE_TYPE ((DRM_RESULT)0x8004C800L)

/*
 * MessageId: DRM_E_BCERT_PUBLIC_KEY_NOT_SPECIFIED
 *
 * MessageText:
 *
 * A public key for the certificate was not provided to the builder or not found by the parser
 *
 */
#define DRM_E_BCERT_PUBLIC_KEY_NOT_SPECIFIED ((DRM_RESULT)0x8004C817L)

/*
 * MessageId: DRM_E_BCERT_TOO_MANY_PUBLIC_KEYS
 *
 * MessageText:
 *
 * There are too many public keys in the certificate
 *
 */
#define DRM_E_BCERT_TOO_MANY_PUBLIC_KEYS ((DRM_RESULT)0x8004C83AL)

/*
 * MessageId: DRM_E_NO_URL
 *
 * MessageText:
 *
 * Can not find URL info.
 *
 */
#define DRM_E_NO_URL                     ((DRM_RESULT)0x8004C05DL)

/*
 * MessageId: DRM_E_BCERT_INVALID_MAX_HEADER_SIZE
 *
 * MessageText:
 *
 * An invalid maximum license header size value was encountered
 *
 */
#define DRM_E_BCERT_INVALID_MAX_HEADER_SIZE ((DRM_RESULT)0x8004C809L)

/*
 * MessageId: DRM_E_BCERT_INVALID_MAX_LICENSE_CHAIN_DEPTH
 *
 * MessageText:
 *
 * An invalid maximum license chain depth was encountered
 *
 */
#define DRM_E_BCERT_INVALID_MAX_LICENSE_CHAIN_DEPTH ((DRM_RESULT)0x8004C80AL)

/*
 * MessageId: DRM_E_BCERT_METERING_URL_TOO_LONG
 *
 * MessageText:
 *
 * The metering URL contains too many ASCII characters
 *
 */
#define DRM_E_BCERT_METERING_URL_TOO_LONG ((DRM_RESULT)0x8004C835L)

/*
 * MessageId: DRM_E_BCERT_INVALID_MAX_KEY_USAGES
 *
 * MessageText:
 *
 * Number of key usage entries is invalid
 *
 */
#define DRM_E_BCERT_INVALID_MAX_KEY_USAGES ((DRM_RESULT)0x8004C81FL)

/*
 * MessageId: DRM_E_BCERT_REQUIRED_KEYUSAGE_MISSING
 *
 * MessageText:
 *
 * Required key usage is missing
 *
 */
#define DRM_E_BCERT_REQUIRED_KEYUSAGE_MISSING ((DRM_RESULT)0x8004C837L)

/*
 * MessageId: DRM_E_BCERT_EXTDATA_IS_NOT_PROVIDED
 *
 * MessageText:
 *
 * Extended data is not provided, the cert builder cannot write it.
 *
 */
#define DRM_E_BCERT_EXTDATA_IS_NOT_PROVIDED ((DRM_RESULT)0x8004C82EL)

/*
 * MessageId: DRM_E_BCERT_OBJECTHEADER_LEN_TOO_SMALL
 *
 * MessageText:
 *
 * Object length in object header is too small
 *
 */
#define DRM_E_BCERT_OBJECTHEADER_LEN_TOO_SMALL ((DRM_RESULT)0x8004C83BL)

/*
 * MessageId: DRM_E_BCERT_INVALID_CHAIN_HEADER_TAG
 *
 * MessageText:
 *
 * Cert chain header tag is invalid
 *
 */
#define DRM_E_BCERT_INVALID_CHAIN_HEADER_TAG ((DRM_RESULT)0x8004C821L)

/*
 * MessageId: DRM_E_BCERT_INVALID_CHAIN_VERSION
 *
 * MessageText:
 *
 * Cert chain version is invalid
 *
 */
#define DRM_E_BCERT_INVALID_CHAIN_VERSION ((DRM_RESULT)0x8004C822L)

/*
 * MessageId: DRM_E_BCERT_NO_PUBKEY_WITH_REQUESTED_KEYUSAGE
 *
 * MessageText:
 *
 * The certificate does not contain a public key with the requested key usage
 *
 */
#define DRM_E_BCERT_NO_PUBKEY_WITH_REQUESTED_KEYUSAGE ((DRM_RESULT)0x8004C838L)

/*
 * MessageId: DRM_E_BCERT_VERIFICATION_ERRORS
 *
 * MessageText:
 *
 * Verification errors are found while parsing cert chain
 *
 */
#define DRM_E_BCERT_VERIFICATION_ERRORS  ((DRM_RESULT)0x8004C836L)

/*
 * MessageId: DRM_E_BCERT_BASICINFO_CERT_EXPIRED
 *
 * MessageText:
 *
 * Certificate is expired
 *
 */
#define DRM_E_BCERT_BASICINFO_CERT_EXPIRED ((DRM_RESULT)0x8004C81CL)

/*
 * MessageId: DRM_E_BCERT_HWIDINFO_IS_MISSING
 *
 * MessageText:
 *
 * The PC certificate is correct but is not ready to use because has no HWID information
 *
 */
#define DRM_E_BCERT_HWIDINFO_IS_MISSING  ((DRM_RESULT)0x8004C82FL)

/*
 * MessageId: DRM_E_NOMORE
 *
 * MessageText:
 *
 * No more data is available.
 *
 */
#define DRM_E_NOMORE                     ((DRM_RESULT)0x80070103L)

/* ------------------------------------------------------------------------- */

#endif /* __DRMBCERT_H__ */

