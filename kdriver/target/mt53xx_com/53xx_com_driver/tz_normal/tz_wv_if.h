/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/

#ifndef TZ_WV_H 
#define TZ_WV_H
#include "x_typedef.h"
//#include <linux/types.h>


/*-----------------------------------------------------------------------------
                    macros, typedefs, enums
 ----------------------------------------------------------------------------*/

#define kPssSaltLength  20
#define KEY_CONTROL_SIZE  16
#define DEVICE_KEY_LENGTH  16
#define KEY_DATA_LENGTH  72
#define DEVICE_ID_LENGTH  32
#define CNT_KEY_DATA_LENGTH  32

#define MAX_SESSION_KEYPAIR  50
#define MACKEY_LEN  32
#define ENCKEY_LEN  16
#define SESSKEY_LEN  32
#define KEY_SIZE  16
//for the nonce table
#define kTableSize  16
#define KEY_ID_LENGTH  16
#define KEY_IV_SIZE  16
#define MAC_KEY_SIZE  32

#define RSA_KEY_LEN 256
#define SHA256_DIGEST_LENGTH 32
#define SHA_DIGEST_LENGTH 20
#define AES_BLOCK_SIZE 16
#define AES_FUN_BLK_LEN      (16)


typedef uint8_t KeyData[CNT_KEY_DATA_LENGTH];
typedef uint8_t KeyId[KEY_ID_LENGTH];

#if 1
typedef struct Rsa_key {
    /** Type of key, PK_PRIVATE or PK_PUBLIC */
    int type;
    /** The public exponent */
    void *e; 
    /** The private exponent */
    void *d; 
    /** The modulus */
    void *N; 
    /** The p factor of N */
    void *p; 
    /** The q factor of N */
    void *q; 
    /** The 1/q mod p CRT param */
    void *qP; 
    /** The d mod (p - 1) CRT param */
    void *dP; 
    /** The d mod (q - 1) CRT param */
    void *dQ;
} rsa_key;
#endif

typedef enum OEMCryptoResult {
    OEMCrypto_SUCCESS                            = 0,
    OEMCrypto_ERROR_INIT_FAILED                  = 1,
    OEMCrypto_ERROR_TERMINATE_FAILED             = 2,
    OEMCrypto_ERROR_OPEN_FAILURE                 = 3,
    OEMCrypto_ERROR_CLOSE_FAILURE                = 4,
    OEMCrypto_ERROR_ENTER_SECURE_PLAYBACK_FAILED = 5,
    OEMCrypto_ERROR_EXIT_SECURE_PLAYBACK_FAILED  = 6,
    OEMCrypto_ERROR_SHORT_BUFFER                 = 7,
    OEMCrypto_ERROR_NO_DEVICE_KEY                = 8,
    OEMCrypto_ERROR_NO_ASSET_KEY                 = 9,
    OEMCrypto_ERROR_KEYBOX_INVALID               = 10,
    OEMCrypto_ERROR_NO_KEYDATA                   = 11,
    OEMCrypto_ERROR_NO_CW                        = 12,
    OEMCrypto_ERROR_DECRYPT_FAILED               = 13,
    OEMCrypto_ERROR_WRITE_KEYBOX                 = 14,
    OEMCrypto_ERROR_WRAP_KEYBOX                  = 15,
    OEMCrypto_ERROR_BAD_MAGIC                    = 16,
    OEMCrypto_ERROR_BAD_CRC                      = 17,
    OEMCrypto_ERROR_NO_DEVICEID                  = 18,
    OEMCrypto_ERROR_RNG_FAILED                   = 19,
    OEMCrypto_ERROR_RNG_NOT_SUPPORTED            = 20,
    OEMCrypto_ERROR_SETUP                        = 21,
    OEMCrypto_ERROR_OPEN_SESSION_FAILED          = 22,
    OEMCrypto_ERROR_CLOSE_SESSION_FAILED         = 23,
    OEMCrypto_ERROR_INVALID_SESSION              = 24,
    OEMCrypto_ERROR_NOT_IMPLEMENTED              = 25,
    OEMCrypto_ERROR_NO_CONTENT_KEY               = 26,
    OEMCrypto_ERROR_CONTROL_INVALID              = 27,
    OEMCrypto_ERROR_UNKNOWN_FAILURE              = 28,
    OEMCrypto_ERROR_INVALID_CONTEXT              = 29,
    OEMCrypto_ERROR_SIGNATURE_FAILURE            = 30,
    OEMCrypto_ERROR_TOO_MANY_SESSIONS            = 31,
    OEMCrypto_ERROR_INVALID_NONCE                = 32,
    OEMCrypto_ERROR_TOO_MANY_KEYS                = 33,
    OEMCrypto_ERROR_DEVICE_NOT_RSA_PROVISIONED   = 34,
    OEMCrypto_ERROR_INVALID_RSA_KEY              = 35,
} OEMCryptoResult;


typedef enum BufferType
{
	kBufferTypeClear,
	kBufferTypeSecure,
	kBufferTypeDirect
}BufferType;


typedef enum E_KeyboxError
{
	NO_ERROR,
	BAD_CRC,
	BAD_MAGIC,
	OTHER_ERROR
}E_KeyboxError;

typedef struct
{
	uint8_t signature[MAC_KEY_SIZE];
	uint8_t context[MAC_KEY_SIZE];
	uint8_t iv[KEY_IV_SIZE];
	uint8_t enc_rsa_key[];
} WrappedRSAKey;


typedef enum KeyType
{
    KEYTYPE_UNKNOWN,
    KEYTYPE_PREPROV,
    KEYTYPE_ROOT,
    KEYTYPE_DEVICE,
    KEYTYPE_CONTENT,
    KEYTYPE_CONTENT_AUDIO,
    KEYTYPE_CONTENT_VIDEO,
    KEYTYPE_MAX
}KeyType;

typedef struct KeyControlBlock
{
    uint32_t verification_;
    uint32_t duration_;
    uint32_t nonce_;
    uint32_t control_bits_;
    BOOL valid_;
}KeyControlBlock;

// AES-128 crypto key
typedef struct Key
{
    BOOL    valid_;
    KeyType type_;
    KeyData keyvalue_;
    int     keydatalen_;
    BOOL    has_control_;
    KeyControlBlock control_;
}Key;

typedef struct Keypair
{
    KeyId   keyid;
    Key     keycontent;
    BOOL    is_valid;
}Keypair;

typedef enum E_NonceTableState
{
    e_kNTStateInvalid,
    e_kNTStateValid,
    e_kNTStateFlushPending
}E_NonceTableState;

typedef struct NonceTable
{
    E_NonceTableState state_[kTableSize];
    uint32_t age_[kTableSize];
    uint32_t nonces_[kTableSize];
}NonceTable;

typedef struct SessionContext
{
    BOOL valid_;
    BOOL has_mac_key;
    uint8_t mac_key_server_[MACKEY_LEN];

    uint8_t mac_key_client_[MACKEY_LEN];
    uint8_t encryption_key_[ENCKEY_LEN];
    size_t  encryp_key_len_;
    uint8_t session_key_[SESSKEY_LEN];

    const Key* current_content_key_;
    Keypair session_keys_[MAX_SESSION_KEYPAIR];
    NonceTable nonce_table_;
    rsa_key* rsa_key_;    //liufen del for build
}SessionContext;

/*
 * OEMCrypto_Algorithm
 * This is a list of valid algorithms for OEMCrypto_Generic_* functions.
 * Some are valid for encryption/decryption, and some for signing/verifying.
 */
typedef enum OEMCrypto_Algorithm
{
	OEMCrypto_AES_CBC_128_NO_PADDING = 0,
	OEMCrypto_HMAC_SHA256			 = 1,
} OEMCrypto_Algorithm;


typedef struct WV_IOCTL_DERIVEKEY_T
{
    //SessionContext*  SessionContext;
    uint32_t   SessionContext;
	const uint8_t*    master_key;	
	size_t mst_cnt_len;
	const uint8_t*    mac_context;
	size_t mac_cnt_len;
	const uint8_t*    enc_context;	
    size_t enc_cnt_len;
} WV_IOCTL_DERIVEKEY_T;

typedef struct WV_IOCTL_GENERIC_T
{
	//SessionContext*  SessionContext;
	uint32_t   SessionContext;
	uint8_t* in_buffer;
	size_t in_buffer_length;
	OEMCrypto_Algorithm algorithm;
	uint8_t* signature;
	size_t signature_length;
	uint32_t    u4Curtime;
} WV_IOCTL_GENERIC_T;

typedef struct WV_IOCTL_NONCE_T
{
	//SessionContext*  SessionContext;
	uint32_t   SessionContext;
	uint32_t   u4Nonce;
} WV_IOCTL_NONCE_T;

typedef struct WV_IOCTL_SIG_T
{
	//SessionContext*  SessionContext;
	uint32_t   SessionContext;
	uint8_t* message;
	size_t message_length;
	uint8_t* signature;
	size_t signature_length;
} WV_IOCTL_SIG_T;

typedef struct WV_IOCTL_KEY_T
{
	//SessionContext*  SessionContext;
	uint32_t   SessionContext;
	uint8_t* key_id;
	size_t key_id_length;
	uint8_t* key_data;
	size_t key_data_length;
	uint8_t* key_data_iv;
	size_t key_data_iv_length;
	uint8_t* key_control;
	size_t key_control_length;
	uint8_t* key_control_iv;
	size_t key_control_iv_length;
} WV_IOCTL_KEY_T;

typedef struct WV_IOCTL_MACKEY_T
{
	//SessionContext*  SessionContext;
	uint32_t   SessionContext;
	uint8_t* mac_keys;
	size_t mac_keys_length;
	uint8_t* iv;
	size_t iv_length;
} WV_IOCTL_MACKEY_T;

typedef struct WV_IOCTL_CONTKEY_T
{
	//SessionContext*  SessionContext;
	uint32_t   SessionContext;
	uint8_t* key_id;
	size_t key_id_length;
} WV_IOCTL_CONTKEY_T;

typedef struct WV_IOCTL_RSAKEY_T
{
	//SessionContext*  SessionContext;
	uint32_t   SessionContext;
	uint8_t* message;
	size_t message_length;
	uint8_t* signature;
	size_t signature_length;
	uint32_t nonce;
	uint8_t* enc_rsa_key; 
	size_t enc_rsa_key_length;
	uint8_t* enc_rsa_key_iv;
	uint8_t* wrapped_rsa_key;
	size_t  wrapped_rsa_key_length;
	uint32_t result;
}WV_IOCTL_RSAKEY_T;

typedef struct WV_IOCTL_RSADERIVEKEY_T
{
	//SessionContext*  SessionContext;
	uint32_t   SessionContext;
	uint8_t* enc_session_key;
	size_t enc_session_key_length;
	uint8_t* mac_context;
	size_t mac_context_length;
	uint8_t* enc_context;
	size_t enc_context_length;
} WV_IOCTL_RSADERIVEKEY_T;


typedef struct WV_IOCTL_DEC_CTR_T
{
	//SessionContext*  SessionContext;
	uint32_t   SessionContext;
	const uint8_t*    iv;
	size_t      block_offset;
	const uint8_t*    cipher_data;
	size_t      cipher_data_length;
	uint32_t        is_encrypted;//bool        is_encrypted;
	void*       clear_data;
	BufferType  buffer_type;
	uint32_t	u4Curtime;
} WV_IOCTL_DEC_CTR_T;

typedef struct WV_IOCTL_GEN_T
{
	//SessionContext*  SessionContext;
	uint32_t   SessionContext;
	const uint8_t*    in_buffer;
	size_t      buffer_length;
	const uint8_t*    iv;
	OEMCrypto_Algorithm algorithm;
	uint8_t*	out_buffer;
	uint32_t	u4Curtime;
} WV_IOCTL_GEN_T;

/*	
typedef struct WV_IOCTL_KEY_INFO_T
{
	uint8_t*	key_buff;
	size_t*		key_len;
	int 		i4Ret;
} WV_IOCTL_KEY_INFO_T;
*/

typedef struct WV_IOCTL_SESS_T
{
	uint32_t 	sessaddr;
	
}WV_IOCTL_SESS_T;


typedef struct WV_IOCTL_CHK_KEY_T
{
	//SessionContext*  SessionContext;
	uint32_t   SessionContext;
	uint32_t		u4Curtime;
	BufferType buffer_type;
	unsigned short  tem_slot;
	int         enc_dec;
} WV_IOCTL_CHK_KEY_T;


///////////////////////////////////for hulu+dash+wv drm only begin//////////////
EXTERN BOOL TZ_DRM_WV_ValidateKeybox(UINT32 *result);
EXTERN BOOL TZ_DRM_WV_CreatSession(UINT32 *sesscnt);
EXTERN BOOL TZ_DRM_WV_DestroySession(SessionContext *sesscnt);
EXTERN BOOL TZ_DRM_WV_GenerateNonceSess(SessionContext * sesscnt, uint32_t * nonce);
EXTERN BOOL TZ_DRM_WV_GenerateDeriveKeys(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_DRM_WV_GenerateSignature(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_DRM_WV_ValidateMessage(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_DRM_WV_InstallKey(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_DRM_WV_FlushNoncesSess(SessionContext * sesscnt);
EXTERN BOOL TZ_DRM_WV_UpdateMacKeys(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_DRM_WV_RefreshKey(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_DRM_WV_SelectContentKey(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_DRM_WV_CheckDecCTRContentKey(SessionContext *sesscnt,uint32_t curtime,BufferType buffer_type,unsigned short * tmp_slot);
EXTERN BOOL TZ_DRM_WV_CheckEncDecCBCContentKey(SessionContext *sesscnt,uint32_t curtime,int enc_dec,unsigned short * tmp_slot);
#if 0
EXTERN BOOL TZ_DRM_WV_DecryptCTR(SessionContext* session,
                const uint8_t* iv,
                size_t block_offset,
                const uint8_t* cipher_data,
                size_t cipher_data_length,
                bool is_encrypted,
                void* clear_data,
                BufferType buffer_type,uint32_t curtime);
EXTERN BOOL TZ_DRM_WV_Generic_Encrypt(SessionContext *sesscnt, const uint8_t* in_buffer,
                     size_t buffer_length,
                     const uint8_t* iv,
                     OEMCrypto_Algorithm algorithm,
                     uint8_t* out_buffer, uint32_t curtime);
EXTERN BOOL TZ_DRM_WV_Generic_Decrypt(SessionContext *sesscnt, const uint8_t* in_buffer,
                     size_t buffer_length,
                     const uint8_t* iv,
                     OEMCrypto_Algorithm algorithm,
                     uint8_t* out_buffer, uint32_t curtime);
#endif
EXTERN BOOL TZ_DRM_WV_Generic_Sign(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_DRM_WV_Generic_Verify(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_DRM_WV_RewrapDeviceRSAKeySess(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_DRM_WV_LoadDeviceRSAKeySessCtx(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_DRM_WV_GenerateRSASignature(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_DRM_WV_RSADeriveKeys(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_DRM_WV_SaveKeybox(UINT8 *pu1Keybox, UINT32 u4Size);
EXTERN BOOL TZ_DRM_WV_GetRandomNum(void *prKernParam, UINT32 u4Size);

///////////////////////////////////for hulu+dash+wv drm only end//////////////


#endif //TZ_WV_H

