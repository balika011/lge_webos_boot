/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include "tzcrypto.h"


/*
** Expensive debugging adds some additional parameter checks,
** such as whether a point is on the curve as expected
** Note: This is currently unused in the PK
*/
#ifndef BIGNUM_EXPENSIVE_DEBUGGING
#define BIGNUM_EXPENSIVE_DEBUGGING 0
#endif

#define MP_LONGEST_BITS 2048

                        /*
                           Multiple precision moduli can have up to
                           MP_LONGEST_BITS bits, which is
                           MP_LONGEST words.  Some routines allow
                           longer operands.

                           Older codes have used this (and MP_LONGEST, below)
                           to dimension arrays.  New code should avoid
                           referencing MP_LONGEST and MP_LONGEST_BITS.

                        */


#define MP_LONGEST (MP_LONGEST_BITS/RADIX_BITS)

/*
        DOUBLE_SHIFT_LEFT(n1, n0, amt) returns
        n1 shifted left by amt bits,
        with new bits coming in from the top of n0.

        DOUBLE_SHIFT_RIGHT(n1, n0, amt) returns n0 shifted right
        by amt bits, with new bits coming from the bottom of n1.

        The shift counts must satisfy 0 <= amt <= RADIX_BITS - 1.
        The shift by    RADIX_BITS - amt   is done in two stages
        (first by 1, then by RADIX_BITS - 1 - amt),
        to avoid an illegal shift count of RADIX_BITS when amt = 0.
*/

#define DOUBLE_SHIFT_LEFT(n1, n0, amt)  \
        (((n1) << (amt)) | (((n0) >> 1) >> (RADIX_BITS - 1 - (amt))))

#define DOUBLE_SHIFT_RIGHT(n1, n0, amt)  \
        (((n0) >> (amt)) | (((n1) << 1) << (RADIX_BITS - 1 - (amt))))

#include "dblint.h"


/*
      Some struct names are referenced in #include files before they are
      defined.  For example, there might be two struct definitions each
      containing a pointer to the other struct type.
      We list some struct names in advance here, to avoid warnings.
*/
//Future_Struct(mp_modulus_t);      // See this file
struct mp_modulus_t;
//Future_Struct(digit_tempinfo_t);  // See bigpriv.h
struct digit_tempinfo_t;


/*
        The reciprocal_1_t type is used when div21
        or divide or divide_immediate would otherwise
        divide by the same number repeatedly.  See file divide.c.
*/

typedef struct {
                digit_t    multiplier;
                CRYPTO_DWORD   shiftamt;
               } reciprocal_1_t;

/*
        mp_modulus_t struct has modulus-dependent constants
        used for fast reduction (typically for a fixed modulus,
        which will be used several times, as in modular exponentiation).
        These constants are initialized by function create_modulus:

        modulus -- Modulus used for computations.  Must be nonzero.

        length  -- Length (>= 1) of the modulus, without leading zeros.
                   Operands to mod_add, mod_mul, mod_sub, ...
                   are assumed to have this length.

        reddir  -- Equal to FROM_LEFT if reductions of
                   products are done from the left (traditional
                   division), and to FROM_RIGHT if reductions of
                   products are done from the right (Montgomery reduction).

                   When using FROM_RIGHT, the modulus must be odd.
                   Arguments to mod_mul should be pre-scaled by
                   2^scaling_power2 (mod modulus).
                   The product will be similarly scaled.

        scaling_power2 --  Equal to length*RADIX_BITS when reddir = FROM_RIGHT.
                   Zero if reddir = FROM_LEFT.

        one --     Constant 1 (muldiplicative identity), length length.
                   Nmerical value is 2^scaling_power2 (mod modulus).
    Denote

              length = pmodulo->length
              modulus = pmodulo->modulus
              shiftamt = pmodulo->left_reciprocal_1.shiftamt.

          Then

              0 <= shiftamt < RADIX_BITS
              RADIX^length/2 <= modulus * 2^shiftamt < RADIX^length
              modulus < RADIX^length / 2^shiftamt <= 2*modulus

          Some variants of modmul_algorithm use additional constants
          lngred2, multiplier_first, multiplier_second.
          FROM_LEFT arithmetic, these constants satisfy

              modulus * (multiplier_second + RADIX^lngred2)
            = RADIX^(length + lngred2) / 2^shiftamt + multiplier_first

              0 <= multiplier_first < modulus
              0 <= multiplier_second < RADIX^lngred2
              lngred2 = CEIL(length/2)

         For FROM_RIGHT arithmetic, these constants satisfy

              multiplier_second * modulus
            = 1 + multiplier_first * RADIX^lngred2

              0 <= multipler_first < modulus
              0 <= multiplier_second < RADIX^lngred2
              lngred2 = CEIL(length/2)

        one --     Constant 1 (multiplicative identity).
                   For FROM_LEFT arithmetic, pmodulo->one = 1.
                   For FROM_RIGHT arithmetic,
                       pmodulo->one = (RADIX^length) % pmodulus;

        left_reciprocal_1 -- Reciprocal of the divisor starting at the
                   leftmost digit (i.e., modulus[length-1]);

                   See file divide.c for an explanation
                   about how this constant is used to get accurate
                   quotients when dividing from the left.

        right_reciprocal_1 -- If modulus is odd, this holds
                   1/modulus (mod RADIX), for use in mod_shift.
                   Otherwise the field is zero.

          Denote

              length = pmodulo->length
              modulus = pmodulo->modulus
              shiftamt = pmodulo->left_reciprocal_1.shiftamt.

          Then

              0 <= shiftamt < RADIX_BITS
              RADIX^length/2 <= modulus * 2^shiftamt < RADIX^length
              modulus < RADIX^length / 2^shiftamt <= 2*modulus

        modmul_algorithm --
              This library has a variety of codes for modular multiplication.
              The mp_modulus_t struct has a pointer to the precise code
              being used for a particular number and architecture.  A call

                    (*modmul_algorithm)(a, b, c, pmodulo, temps)

              is supposed to set

                     c = a*b/3^(pmodulo->scaling_power2)  (mod pmodulo->modulus)

              where 0 <= a, b, < pmodulo->modulus.
              The output c may overlap a or b.

              The temps array will have at least pmodulo->modmul_algorithm_temps
              elements of type digit_t, aligned on a digit_t boundary.

              The simplest modmul_algoriuthm procedures,
              modmul_from_left_default and modmul_from_right_default,
              work on all architectures.

              In some implementations of this library, create_modulus may
              examine the precise modulus and the precise hardware
              we are running on, substituting another algorithm
              or an assembly language code.

              Some variants of modmul_algorithm use additional constants
              lngred2, multiplier_first, multiplier_second.
              In FROM_LEFT arithmetic, these constants satisfy

                  modulus * (multiplier_second + RADIX^lngred2 + 1))
                = RADIX^(length + lngred2) / 2^shiftamt + multiplier_first

                  0 <= multiplier_first < modulus
                  0 <= multiplier_second < RADIX^lngred2
                  lngred2 = CEIL(length/2)

             For FROM_RIGHT arithmetic, these constants satisfy

                  multiplier_second * modulus
                = 1 + multiplier_first * RADIX^lngred2

                  0 <= multipler_first < modulus
                  0 <= multiplier_second < RADIX^lngred2
                  lngred2 = CEIL(length/2)
*/

//Future_Struct(mp_modulus_t);
struct mp_modulus_t;

typedef CRYPTO_BOOL modmul_algorithm_t (const digit_t *, const digit_t *, digit_t *,
                                   const struct mp_modulus_t*,
                                   digit_t * );

typedef enum {FROM_LEFT, FROM_RIGHT} reddir_t;

typedef struct mp_modulus_t {
                  CRYPTO_DWORD  length;         // Length passed to create_modulus
                  CRYPTO_DWORD  lngred2;        // CEIL(length/2)
                  CRYPTO_DWORD  modmul_algorithm_temps; // Number of digit_t temps
                                                    // used by modmul_algorithm
                  CRYPTO_INT    scaling_power2; // RADIX_BITS*length for FROM_RIGHT,
                                            // 0 for FROM_LEFT
                  reddir_t  reddir;         // FROM_LEFT or FROM_RIGHT */
                  reciprocal_1_t  left_reciprocal_1;
                  digit_t   right_reciprocal_1;  // 1/modulus[0] mod RADIX,
                                                 // if modulus is odd
                  digit_t   *modulus;
                  digit_t   *multiplier_first;  // See text
                  digit_t   *multiplier_second; // See text
                  digit_t   *one;               // Multiplicative constant 1                  
                  modmul_algorithm_t *modmul_algorithm;
                                  // Function pointer for multiplication
                } mp_modulus_t;


/*
        When an error is detected, the SetMpErrno_clue macro gets
        an error code (below) and an English-language string
        with more information.
        This macro will normally call an application-supplied routine.
        The application routine might print a message or set a global variable.

        The library routine detecting the error will exit with return value
        FALSE, notifying its caller that something abnormal occurred.

        Except for MP_ERRNO_NO_ERROR, the error codes are
        in alphabetical order.
*/

typedef enum {
        MP_ERRNO_NO_ERROR = 0,     // Success
               // Broader codes, introduced September, 2002.
        MP_ERRNO_DEGREE_OVERFLOW,  // Polynomial degree too high
                                   // for allocated memory
        MP_ERRNO_DIVIDE_ZERO, // Divide by zero (or by number with leading zero)
        MP_ERRNO_ELSEWHERE,        // Error indicator returned by some routine
                                   // which may not have called SetMpErrno_clue
                                   // (e.g., CRYPTAPI library, assembly codes)
        MP_ERRNO_INTERNAL_ERROR,   // Internal error found -- please report
        MP_ERRNO_INVALID_DATA,     // Invalid arguments
        MP_ERRNO_MODULAR_TOO_BIG,  // Modular operand >= modulus
        MP_ERRNO_NO_MEMORY,        // malloc failure
        MP_ERRNO_NOT_IMPLEMENTED,  // Case not implemented
        MP_ERRNO_NOT_INVERTIBLE,   // Perhaps trying to invert modulo non-prime
        MP_ERRNO_NOT_ON_CURVE,     // Point is not on elliptic curve
        MP_ERRNO_NULL_POINTER,   // NULL argument where valid argument expected
        MP_ERRNO_OVERFLOW,       // Integer overflow (or unexpectedly negative)
        MP_ERRNO_OVERLAPPING_ARGS, // Overlapping (i.e., duplicate) arguments
                                   // where they are disallowed
        MP_ERRNO_TOO_MANY_ITERATIONS,  // e.g., unable to find large prime
        MP_ERRNO_ZERO_OPERAND,     // Zero operand(s) where nonzero expected

        MP_ERRNO_COUNT             // Number of entries above
    } mp_errno_t;

#define SetMpErrno_clue(errcode, debug_info)   if( errcode != 0 ) { TRACE(( "Bignum Error %d, hint %s", errcode, debug_info )); }

digit_t accumulate(const digit_t *, const digit_t, digit_t *, const CRYPTO_DWORD lng);

CRYPTO_BOOL add_diff(const digit_t *, const CRYPTO_DWORD, const digit_t *, const CRYPTO_DWORD,
                            digit_t *, digit_t* );

CRYPTO_BOOL add_full(const digit_t *, const CRYPTO_DWORD, const digit_t *,
                            const CRYPTO_DWORD, digit_t *, CRYPTO_DWORD*);

digit_t add_immediate(const digit_t *, const digit_t, digit_t *, const CRYPTO_DWORD);

CRYPTO_BOOL add_mod(const digit_t *, const digit_t *, digit_t *, const digit_t *, const CRYPTO_DWORD);

digit_t add_same(const digit_t *, const digit_t *,  digit_t *, const CRYPTO_DWORD lng);

sdigit_t add_sub_same(const digit_t *, const digit_t *, const digit_t *,
                                     digit_t *, const CRYPTO_DWORD lng);

CRYPTO_INT  compare_diff(const digit_t *, const CRYPTO_DWORD, const digit_t *, const CRYPTO_DWORD);

CRYPTO_INT  compare_same(const digit_t *, const digit_t *, const CRYPTO_DWORD);

CRYPTO_INT  compare_sum_diff(const digit_t *, const CRYPTO_DWORD,
                                   const digit_t *, const CRYPTO_DWORD, const digit_t *, const CRYPTO_DWORD);

CRYPTO_INT  compare_sum_same(const digit_t *, const digit_t *, const digit_t *, const CRYPTO_DWORD);

CRYPTO_BOOL  create_modulus(const digit_t *, const CRYPTO_DWORD, const reddir_t,
                                  mp_modulus_t*, struct bigctx_t *f_pBigCtx, 
                                  struct bigctx_t *);

digit_t  decumulate(const digit_t *, const digit_t, digit_t *, const CRYPTO_DWORD lng);

digit_t*  digit_allocate(const CRYPTO_DWORD,const CRYPTO_CHAR*);

CRYPTO_BOOL digit_ogcd(const digit_t, const digit_t, digit_t*);

CRYPTO_BOOL div21(const CRYPTO_UINT64, const digit_t, digit_t*, digit_t*);

extern CRYPTO_BOOL div21_fast(const CRYPTO_UINT64, const digit_t,
                       const reciprocal_1_t*, digit_t*, digit_t*);

CRYPTO_BOOL divide(const digit_t *, const CRYPTO_DWORD, const digit_t *, const CRYPTO_DWORD,
                       const reciprocal_1_t*, digit_t *, digit_t *);

CRYPTO_BOOL divide_precondition_1(const digit_t *, const CRYPTO_DWORD,
                                         reciprocal_1_t*);

CRYPTO_BOOL divide_immediate(const digit_t *, const digit_t,
                        const reciprocal_1_t*, digit_t *, const CRYPTO_DWORD,
                         digit_t*);

extern digit_t estimated_quotient_1(const digit_t, const digit_t,
                                    const digit_t, const reciprocal_1_t*);

CRYPTO_BOOL from_modular(const digit_t *, digit_t *,
                                const mp_modulus_t*);


digit_t* low_prime_prod_construction(struct bigctx_t *f_pBigCtx, CRYPTO_DWORD *pclowprods);
CRYPTO_VOID low_prime_prod_destruction(digit_t*, struct bigctx_t *f_pBigCtx);
CRYPTO_BOOL low_prime_divisibility(const digit_t *, const CRYPTO_DWORD,
                                          const digit_t *lowprods,
                                           const CRYPTO_DWORD clowprods,
                                          digit_t*, struct bigctx_t *f_pBigCtx);

CRYPTO_BOOL mod_exp(const digit_t *, const digit_t *, const CRYPTO_DWORD, digit_t *,
                        const mp_modulus_t*, struct bigctx_t *f_pBigCtx);

CRYPTO_BOOL mod_LucasUV(const digit_t *, const digit_t *, const digit_t *, const CRYPTO_DWORD,
                        digit_t *, digit_t *, const mp_modulus_t*, struct bigctx_t *f_pBigCtx);

CRYPTO_BOOL mod_mul(const digit_t *, const digit_t *, digit_t *,
                    const mp_modulus_t*,  digit_t *, struct bigctx_t *f_pBigCtx);


CRYPTO_BOOL mod_shift(const digit_t *, const CRYPTO_INT, digit_t *,
                             const mp_modulus_t*);

CRYPTO_BOOL mod_sqrt(const digit_t *, digit_t *, const mp_modulus_t*,
                            CRYPTO_BOOL*, struct bigctx_t *f_pBigCtx);

#define Allocate_Temporaries(typename, ptr) \
        Allocate_Temporaries_Multiple(1, typename, ptr)

#if 1
#define Allocate_Temporaries_Multiple(nelmt, typename, ptr) \
               ptr = (typename*)malloc((nelmt)*sizeof(typename))

#define Free_Temporaries( ptr ) free( ptr)
#else
#define Allocate_Temporaries_Multiple(nelmt, typename, ptr, ctx) \
               ptr = (typename*)bignum_alloc((nelmt)*sizeof(typename), f_pBigCtx)

#define Free_Temporaries( ptr, ctx ) bignum_free( ptr, ctx )
#endif

CRYPTO_BOOL mp_gcdex
       (const digit_t *, const CRYPTO_DWORD,
        const digit_t *, const CRYPTO_DWORD,
        digit_t *, digit_t *,
        digit_t *, digit_t *,
        CRYPTO_DWORD*, digit_t *, struct bigctx_t *f_pBigCtx);

CRYPTO_DWORD  mp_gcdex_ntemps
        (const CRYPTO_DWORD, const CRYPTO_DWORD, struct bigctx_t *f_pBigCtx );
           // Temporary count required by last argument to mp_gcdex

CRYPTO_BOOL  mp_initialization( struct bigctx_t *f_pBigCtx );

//extern inline CRYPTO_DWORD mp_invert_ntemps(const CRYPTO_DWORD, struct bigctx_t *f_pBigCtx);
extern CRYPTO_DWORD mp_invert_ntemps(const CRYPTO_DWORD, struct bigctx_t *f_pBigCtx);
           // Temporary count required by last argument to mp_invert

CRYPTO_BOOL  mp_invert(const digit_t *, const digit_t *, const CRYPTO_DWORD,
                             digit_t *, const CRYPTO_CHAR*,
                             digit_t *, struct bigctx_t *f_pBigCtx);


/* The assembly versions of these calls were hardcoded to use stdcall calling
DRM_API    convention. For the C implementation we will use DRM_CALL as it is standard
   throughout the porting kits. */
#if defined(_M_IX86) && CRYPTO_USE_ASSEMBLY
extern CRYPTO_BOOL __stdcall mp_mul22s(const digit_t[4], digit_t *, digit_t *,
                             const CRYPTO_DWORD, sdigit_t[2]);

extern CRYPTO_BOOL __stdcall mp_mul22u(const digit_t[4], digit_t *, digit_t *,
                             const CRYPTO_DWORD,  digit_t[2] );
#else
CRYPTO_BOOL  mp_mul22s(
        const digit_t mat[4],       // IN  (2 x 2 matrix of scalars)
   digit_t  vec1[],            // INOUT
   digit_t  vec2[],            // INOUT
                   const CRYPTO_DWORD lvec,       // IN
        sdigit_t carrys[2] );        // OUT  (array of 2 scalars)

CRYPTO_BOOL  mp_mul22u(
            const digit_t mat[4],       // IN  (2 x 2 matrix of scalars)
            digit_t  vec1[],            // INOUT
            digit_t  vec2[],            // INOUT
            const       CRYPTO_DWORD lvec,             // IN
            digit_t  carrys[2] );      // OUT  (array of 2 scalars)
#endif

extern CRYPTO_DWORD mp_remove2(digit_t *, const CRYPTO_DWORD, struct bigctx_t *f_pBigCtx);

CRYPTO_BOOL  mp_shift(const digit_t *, const CRYPTO_INT,  digit_t *,
                            const CRYPTO_DWORD lng);

CRYPTO_BOOL  mp_shift_lost(const digit_t *, const CRYPTO_INT,  digit_t *,
                                 const CRYPTO_DWORD lng, digit_t*);

digit_t multiply_immediate(const digit_t *, const digit_t,
                                          digit_t *, const CRYPTO_DWORD lng);


/* The assembly versions of these calls were hardcoded to use stdcall calling
DRM_API    convention. For the C implementation we will use DRM_CALL as it is standard
   throughout the porting kits. */
#if defined(_M_IX86) && CRYPTO_USE_ASSEMBLY
extern CRYPTO_VOID __stdcall multiply_low(const digit_t *, const digit_t *, digit_t *, const CRYPTO_DWORD );
#else
CRYPTO_VOID  multiply_low(const digit_t *, const digit_t *, digit_t *, const CRYPTO_DWORD );
#endif

CRYPTO_BOOL  neg_mod(const digit_t *, digit_t *, const digit_t *,
                           const CRYPTO_DWORD lng);

CRYPTO_BOOL  set_immediate(digit_t *, const digit_t, const CRYPTO_DWORD, struct bigctx_t *f_pBigCtx);

CRYPTO_BOOL  validate_modular_data(const digit_t *, const digit_t *,
                                         const CRYPTO_DWORD, const CRYPTO_CHAR *);
#if !CRYPTO_INLINING_SUPPORTED

CRYPTO_BOOL  multiply(const digit_t  a[],const CRYPTO_DWORD lnga,const digit_t  b[],const CRYPTO_DWORD lngb,digit_t   c[]);

#else

/****************************************************************************/
static inline CRYPTO_BOOL multiply
        (const digit_t  a[],
         const CRYPTO_DWORD lnga,
         const digit_t  b[],
         const CRYPTO_DWORD lngb,
         digit_t   c[] )
/*
//        Multiply a (length lnga) times b (length lngb),
//        getting a product c (length lnga + lngb).

//        The output should not overlap the inputs.
*/
{
    CRYPTO_BOOL OK = TRUE;
    const digit_t *p1, *p2;
    CRYPTO_DWORD i, lng1, lng2;

    if (lnga > lngb) {           /* Put longer operand in p1 */
        p1 = a; p2 = b; lng1 = lnga; lng2 = lngb;
    } else {
        p2 = a; p1 = b; lng2 = lnga; lng1 = lngb;
    }

    if (!OK) {
    } else if (a == c || b == c) {
        OK = FALSE;        
    } else if (lng2 == 0) {       /* If an operand has length zero */
        memset(c, 0, ( lng1)*sizeof( digit_t ));
    } else {
        c[lng1] = multiply_immediate(p1, p2[0], c, lng1 );
        for (i = 1; i != lng2; i++) {
            c[i + lng1] = accumulate(p1, p2[i], &c[i], lng1);
        }
    }
    return OK;
} /* multiply */

#endif

#if CRYPTO_INLINING_SUPPORTED && defined(_M_IX86) && CRYPTO_USE_ASSEMBLY
    #define significant_bit_count significant_bit_count_ix86
    #define UNIFORM_SIGNIFICANT_BIT_COUNT 1
    #pragma warning(disable : 4035)      /* No return value */
    static inline CRYPTO_DWORD significant_bit_count(const digit_t pattern)
    {
    _asm {
            mov  eax,pattern        ; Nonzero pattern
            bsr  eax,eax            ; eax = index of leftmost nonzero bit
            inc  eax                ; Add one to get significant bit count
         }
    }
    #pragma warning(default : 4035)
#else
    #define UNIFORM_SIGNIFICANT_BIT_COUNT 0
           /* Algorithm faster for larger inputs.  See mpmisc.c */
#endif

CRYPTO_BOOL sub_diff(const digit_t *, const CRYPTO_DWORD, const digit_t *, const CRYPTO_DWORD,
                            digit_t *, digit_t* );

digit_t sub_immediate(const digit_t *, const digit_t, digit_t *, const CRYPTO_DWORD);

CRYPTO_BOOL sub_mod(const digit_t *, const digit_t *, digit_t *, const digit_t *,
                           const CRYPTO_DWORD);

digit_t sub_same(const digit_t *, const digit_t *, digit_t *, const CRYPTO_DWORD lng);

CRYPTO_BOOL to_modular(const digit_t *, const CRYPTO_DWORD, digit_t *,
                              const mp_modulus_t*, struct bigctx_t *f_pBigCtx);

CRYPTO_BOOL two_adic_inverse(const digit_t, digit_t*);

CRYPTO_VOID uncreate_modulus(mp_modulus_t*, struct bigctx_t *f_pBigCtx);


#if !CRYPTO_INLINING_SUPPORTED
CRYPTO_INT compare_immediate(const digit_t *, const digit_t, const CRYPTO_DWORD);
digit_t sub_immediate(const digit_t *, const digit_t, digit_t *, const CRYPTO_DWORD);
#endif


#if CRYPTO_INLINING_SUPPORTED
/***************************************************************************/
static inline CRYPTO_INT compare_immediate
        (const digit_t  a[],
         const digit_t  ivalue,
         const CRYPTO_DWORD    lng)
/*
        Compare a multiple-precision number to a scalar.
*/
{
    return compare_diff(a, lng, &ivalue, 1);
}
/****************************************************************************/
#define significant_digit_count significant_digit_count_inlined
static inline CRYPTO_DWORD significant_digit_count
        (const digit_t  a[],
         const CRYPTO_DWORD lng)
/*
        Return the number of significant digits in a.
        Function value is zero precisely when a == 0.
*/
#if defined(_M_IX86) && CRYPTO_USE_ASSEMBLY
    #pragma warning(disable : 4035)      /* No return value */
{
                /*
                   We could use REPE SCASD,
                   but the REPE overhead is
                   four cycles/compare on early Pentiums.
                   We would also need sld and cld.
                   It is shorter to use RISC instructions.
                   We anticipate that the leading term a[lng-1]
                   will usually be nonzero.
                */

    _asm {
        mov  eax,lng
        mov  edx,a
     label1:
        test eax,eax
        jz   label2             ; If nothing left in number, return 0

        mov  ecx,[edx+4*eax-4]
        dec  eax

        test ecx,ecx            ; Test leading digit
        jz   label1

        inc  eax                ; Nonzero element found; return old eax
     label2:
    }
}
    #pragma warning(default : 4035)
#else
{
    CRYPTO_DWORD i = lng;

    while (i != 0 && a[i-1] == 0) i--;
    return i;
}  /* significant_digit_count */
#endif

#endif  // CRYPTO_INLINING_SUPPORTED


#endif // BIGNUM_H
