
#ifndef __CRYPTO_RESULTS_H__
#define __CRYPTO_RESULTS_H__

#define CRYPTO_FAILED(Status) ((CRYPTO_RESULT)(Status)<0)
#define CRYPTO_SUCCEEDED(Status) ((CRYPTO_RESULT)(Status) >= 0)

/*
 * MessageId:CRYPTO_E_P256_ECDSA_VERIFICATION_ERROR
 *
 * MessageText:
 *
 * The ECDSA verification algorithm encountered an unknown error.
 *
 */
#define CRYPTO_E_P256_ECDSA_VERIFICATION_ERROR ((CRYPTO_RESULT)0x80040E8BL)

/*
 * MessageId:CRYPTO_E_P256_ECDSA_SIGNING_ERROR
 *
 * MessageText:
 *
 * The ECDSA signature algorithm encountered an unknown error.
 *
 */
#define CRYPTO_E_P256_ECDSA_SIGNING_ERROR   ((CRYPTO_RESULT)0x80040E8CL)


/*
 * MessageId:CRYPTO_E_P256_INVALID_SIGNATURE
 *
 * MessageText:
 *
 * The ECDSA signature to be verified was not a valid signature format.
 *
 */
#define CRYPTO_E_P256_INVALID_SIGNATURE     ((CRYPTO_RESULT)0x80040E8AL)


/*
 * MessageId:CRYPTO_E_P256_CONVERSION_FAILURE
 *
 * MessageText:
 *
 * An error occurred while converting between P256 types.
 *
 */
#define CRYPTO_E_P256_CONVERSION_FAILURE    ((CRYPTO_RESULT)0x80040E87L)

/*
 * MessageId:CRYPTO_E_P256_PLAINTEXT_MAPPING_FAILURE
 *
 * MessageText:
 *
 * An error occurred while attempting to map a plaintext array to a EC Point: There is no conversion for this byte array to a EC Point.
 *
 */
#define CRYPTO_E_P256_PLAINTEXT_MAPPING_FAILURE ((CRYPTO_RESULT)0x80040E89L)


/*
 * MessageId:CRYPTO_E_P256_PKCRYPTO_FAILURE
 *
 * MessageText:
 *
 * An error occurred in an asymmetric P256 cryptographic operation.
 *
 */
#define CRYPTO_E_P256_PKCRYPTO_FAILURE      ((CRYPTO_RESULT)0x80040E88L)


/*
 * MessageId:CRYPTO_E_P256_HMAC_KEYGEN_FAILURE
 *
 * MessageText:
 *
 * Could not generate a valid HMAC key under constraint where CK || HMACK is a valid x coord on the EC (P256).
 *
 */
#define CRYPTO_E_P256_HMAC_KEYGEN_FAILURE   ((CRYPTO_RESULT)0x80040E8DL)

/*
 * MessageId: DRM_E_INVALIDARG
 *
 * MessageText:
 *
 * The parameter is incorrect.
 *
 */
#define CRYPTO_E_INVALIDARG                 ((CRYPTO_RESULT)0x80070057L)

/*
 * MessageId: DRM_E_PKCRYPTO_FAILURE
 *
 * MessageText:
 *
 * An error occurred in an asymmetric cryptographic operation.
 *
 */
#define CRYPTO_E_PKCRYPTO_FAILURE           ((CRYPTO_RESULT)0x800480D5L)


#define ChkArg(expr) {               \
            if ( !(expr) )       \
            {                       \
                Printf("Invalid argument at %s : %d.\n%s\n", __FILE__, __LINE__, #expr) ; \
                dr = CRYPTO_E_INVALIDARG; \
                goto ErrorExit;     \
            }                       \
        }
        

#define ChkDR(expr) {               \
            dr = (expr);            \
            if ( CRYPTO_FAILED(dr) )   \
            {                       \
                goto ErrorExit;     \
            }                       \
        }
        

#define ChkBOOL(fExpr,err){  \
            if (!(fExpr))            \
            {\
                dr = (err);\
                goto ErrorExit;     \
            }\
        }
        
#define ChkOverflow(a,b) do {                       \
            if (a < b)                              \
            {                                       \
                ChkDR(DRM_E_ARITHMETIC_OVERFLOW);   \
            }                                       \
        } while(FALSE)        
#endif  //__CRYPTO_RESULTS_H__