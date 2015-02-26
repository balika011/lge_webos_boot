/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
#ifndef __FIELD_H__
#define __FIELD_H__

#include "tzcrypto.h"
#include "bignum.h"


/*
        This file defines field_desc_t, a struct representing a field.
        The data structures allow
        GF(2^m) or GF(q) (with multiple precision q).
        finite-degree extensions.

        The letter `K' is often used in mathematics
        to denote a field.  We use names like Kadd
        for field addition, since the name `Fadd'
        suggests a floating point addition routine.

        A field element is an array of type digit_t.
        The elng element of the field_sesc_t struct tells its length

    Arithmetic routines:

        Kadd(f1, f2, f3, &fdesc) -- f3 = f1 + f2
        Kdiv(f1, f2, f3, &fdesc, ftemps) -- f3 = f1 / f2
                            Array of fdesc->ndigtemps_arith temps supplied
        Kequal (f1, f2,  &fdesc) -- Is f1 == f2?        
        Kimmediate(scalar, f1, &fdesc) -- f1 = scalar (a signed digit_t)
        Kinvert(f1, f2,  &fdesc, ftemps) -- f2 = 1/f1
                            Array of fdesc-->ndigtemps_invert1 supplied
        Kiszero(f1,      &fdesc) -- Is f1 == 0?
        Kmulpower2(f1, ishift, f3, fdesc) -- f3 = f1 * 2^ishift
        Kmul(f1, f2, f3, &fdesc) -- f3 = f1 * f2
        Kmul(f1, f2, f3, &fdesc, ftemps) -- f3 = f1*f2,
                        Array of fdesc->ndigtemps_mul temps supplied        
        Kmuladd(f1, f2, f3, f4, &fdesc, ftemps) -- f4 = f1 * f2 + f3
                            Array of fdesc->ndigtemps_arith temps supplied
        Knegate(f1, f2,  &fdesc) -- f2 = -f1
        Ksqrt(f1, f2,    &fdesc) -- f2 = sqrt(f1) (either root)
        Ksub(f1, f2, f3, &fdesc) -- f3 = f1 - f2

    Miscellaneous routines:

        Kclear_many(f1, nelmt,    &fdesc) -- Set nelmt elements to zero.               
        Kfree  (&fdesc)                -- Free any memory malloc-ed
                                          when field was initialized.     
        Kinitalize_normal2(m, T, &fdesc) -- Initialize for normal basis.
        
        Kinitialize_prime(&modulus, &fdesc)
                                       -- Initialize field with prime modulus.
*/


typedef enum {FIELD_TYPE_INVALID = 0,
              FIELD_Q_MP,          // Field mod multiple-precision prime q
              FIELD_2_NORMAL,      // Field GF(2^m) with normal basis over GF(2)
              FIELD_2_POLYNOMIAL}  // Field GF(2^m) with polynomial basis
                                   //   over GF(2)
           field_type_t;

#define CHARACTERISTIC_2(fdesc) ((fdesc)->ftype >= FIELD_2_NORMAL)
                         // Test for characteristic 2 field.

typedef digit_t flongest_t[MP_LONGEST];


   //   Special GF(2^m) fields defined by sparse polynomials.

typedef CRYPTO_BOOL gf2_reduction_t(digit_t *, struct bigctx_t *f_pBigCtx);

//Future_Struct(field_desc_t);
struct field_desc_t;

typedef CRYPTO_BOOL field_0op_notemps_t (digit_t *, const CRYPTO_DWORD,
                                      const struct field_desc_t*, struct bigctx_t *f_pBigCtx);
typedef CRYPTO_BOOL field_1op_notemps_t (const digit_t *, digit_t *,
                                      const CRYPTO_DWORD, const struct field_desc_t*,
                                      struct bigctx_t *f_pBigCtx);
typedef CRYPTO_BOOL field_2op_notemps_t (const digit_t *, const digit_t *, digit_t *,
                                      const CRYPTO_DWORD, const struct field_desc_t*,
                                      struct bigctx_t *f_pBigCtx);


typedef struct {
          field_2op_notemps_t *adder;       // x + y
          CRYPTO_BOOL        (*equaler)     (const digit_t *, const digit_t *,
                                             const CRYPTO_DWORD,
                                             const struct field_desc_t*,
                                             struct bigctx_t *f_pBigCtx);
                                            // Does x == y?

                                            // Compute base^exponent
                                            // (exponent an integer)
          CRYPTO_BOOL        (*freer)       (struct field_desc_t*, struct bigctx_t *f_pBigCtx);
                                             // Free ftype-dependent data when
                                             // Kfree called.
          CRYPTO_BOOL        (*immediater)  (const sdigit_t*, digit_t *,
                                             const CRYPTO_DWORD,
                                             const struct field_desc_t*,
                                             struct bigctx_t *f_pBigCtx);

          CRYPTO_BOOL        (*inverter1)   (const digit_t *, digit_t *,
                                             const struct field_desc_t*,
                                             const struct digit_tempinfo_t*,
                                             struct bigctx_t *f_pBigCtx);
          CRYPTO_BOOL        (*iszeroer)    (const digit_t *, const CRYPTO_DWORD,
                                             const struct field_desc_t*,
                                             struct bigctx_t *f_pBigCtx);
                                            // Does x == 0?
          CRYPTO_BOOL        (*mulpower2er) (const digit_t *, const CRYPTO_INT,
                                             digit_t *, const CRYPTO_DWORD,
                                             const struct field_desc_t*,
                                             struct bigctx_t *f_pBigCtx);
                                            // Multiply by 2^(second argument)
          CRYPTO_BOOL        (*multiplier1) (const digit_t *, const digit_t *,
                                             digit_t *,
                                             const struct field_desc_t*,
                                             digit_t *, struct bigctx_t *f_pBigCtx);
                                            // x * y

          field_1op_notemps_t *negater;     // -x

          CRYPTO_BOOL        (*sizer) (digit_t *,  const struct field_desc_t*,
                                       struct bigctx_t *f_pBigCtx);
                                // Return size of field (array length elng+1)

          field_2op_notemps_t *subtracter;   // x - y
          field_0op_notemps_t *zeroizer;     // 0
} field_arithmetic_t;

typedef struct field_desc_t {
          CRYPTO_DWORD     elng;   // Length of each field element, in digit_t's
          CRYPTO_DWORD     degree; // Extension degree m if GF(2^m)
                               // Also used for extension fields
          CRYPTO_DWORD     ndigtemps_arith;  // Number of digit_t temporaries
                                         // adequate for any of the following:

                                         //   (use subfield->ndigtemps_arith)
                                         // Kdiv
                                         // Kinvert
                                         // Kinvert_many
                                         // Kmul
                                         // Kmuladd
          CRYPTO_DWORD     ndigtemps_mul;
                               // Number of digit_t temporaries
                               // needed for a multiplication.
          CRYPTO_DWORD     ndigtemps_invert1;
                               // Number of digit_t temporaries
                               // needed for an inversion.
          field_type_t ftype;  // Type of base field
          CRYPTO_BOOL         free_modulus;   // If TRUE, Kfree frees
                                       // the modulo field.
                                       // Can be set by application.
          digit_t      *one;   // Constant 1
          digit_t      *deallocate;

          const field_arithmetic_t *arithmetic;
                                     // Pointers to arithmetic routines

// Next items apply only for extension fields
          const struct field_desc_t *base_field;  // Recursive data type
                                 // NULL if not an extension field.
// Next item applies for extension fields and FIELD_2_POLYNOMIAL
          digit_t      *minimal_polynomial;

// Next items apply only if ftype = FIELD_Q_MP
          const mp_modulus_t *modulo;      // Information about q
          digit_t *inverse_adjustment;
                                      // Multiplier to adjust reciprocal
                                      // for FROM_RIGHT arithmetic
// Next items apply only if ftype = FIELD_2_NORMAL
          CRYPTO_DWORD     T;      // See field2n.c
          CRYPTO_DWORD     lng_mulshifts;
          struct k2nmulshifts_t *mulshifts;
                               // Pairs of shifts.  See field2n.c
// Next items apply only if ftype = FIELD_2_POLYNOMIAL or extension field.
//
          CRYPTO_DWORD     nonzero_trace_power;
                               // Chosen so trace(alpha^nonzero_trace_power) = 1
          digit_t      *leading_inverse;
                     // FLOOR(X^(2*degree-1) / minimal_polynomial)
                     // N.B. Either leading_inverse = NULL or freduc = NULL.
// For extension fields, nonzero_trace_power is chosen so
//     minimal_polynomial = X^degree
//        + (nonzero const)*X^(nonzero_trace_power - 1) + ...
//     That is, it counts 1 + degree(minimal polynomial, minus leading term).

//     leading_inverse has signed bytes converting these low coefficients
//     into signed integers in [-16, 16], if possible.

          gf2_reduction_t  *freduc;

// Following apply to FIELD_2_NORMAL and FIELD_2_POLYNOMIAL.

          CRYPTO_DWORD     unused_bits_top;  // elng*RADIX_BITS - degree
          digit_t      mask_used_bits;
                                  // Mask to avoid extraneous bits in
                                  // most significant word of a field element
} field_desc_t;


#define Kadd_many(f1, f2, f3, nelmt, fdesc, ctx) \
        (((fdesc)->arithmetic->adder) ((f1), (f2), (f3), (nelmt), (fdesc), ctx))
#define Kclear_many(      f3, nelmt, fdesc, ctx) \
        (((fdesc)->arithmetic->zeroizer)  ((f3), (nelmt), (fdesc), ctx))
#define Kequal_many(f1, f2,   nelmt, fdesc, ctx) \
        (((fdesc)->arithmetic->equaler)   ((f1), (f2), (nelmt), (fdesc), ctx))

#define Kimmediate_many(sarray, f3, nelmt, fdesc, ctx) \
        (((fdesc)->arithmetic->immediater) \
        ((sarray), (f3), (nelmt), (fdesc), ctx))
#define Kiszero_many(f1,      nelmt, fdesc, ctx) \
        (((fdesc)->arithmetic->iszeroer)  ((f1), (nelmt), (fdesc), ctx))
#define Kmulpower2_many(f1, ishift, f3, nelmt, fdesc, ctx) \
        (((fdesc)->arithmetic->mulpower2er) \
        ( (f1), (ishift), (f3), (nelmt), (fdesc), ctx))
#define Knegate_many(f1,  f3, nelmt, fdesc, ctx) \
        (((fdesc)->arithmetic->negater)   ((f1), (f3), (nelmt), (fdesc), ctx))
#define Ksqrt_many(f1,    f3, nelmt, fdesc, are_squares, ctx) \
        (((fdesc)->arithmetic->sqrter) \
        ((f1), (f3), (nelmt), (fdesc), (are_squares), ctx))
#define Ksub_many(f1, f2, f3, nelmt, fdesc, ctx) \
        (((fdesc)->arithmetic->subtracter) \
        ((f1), (f2), (f3), (nelmt), (fdesc), ctx))


#define Kadd(f1, f2, f3, fdesc, ctx)   \
        Kadd_many(   (f1), (f2), (f3), 1, (fdesc), ctx)
#define Kequal(f1, f2,   fdesc, ctx)   \
        Kequal_many( (f1), (f2),       1, (fdesc), ctx)

#define Kiszero(f1,      fdesc, ctx)   \
        Kiszero_many((f1),             1, (fdesc), ctx)
#define Kmulpower2(f1, ishift, f3, fdesc, ctx)\
        Kmulpower2_many((f1), (ishift), (f3), 1, (fdesc), ctx)
#define Knegate(f1,  f3, fdesc, ctx)   \
        Knegate_many((f1),       (f3), 1, (fdesc), ctx)
#define Ksqrt(f1,    f3, fdesc, is_square, ctx)  \
        Ksqrt_many(  (f1),       (f3), 1, (fdesc), (is_square), ctx)
#define Ksub(f1, f2, f3, fdesc, ctx)   \
        Ksub_many(   (f1), (f2), (f3), 1, (fdesc), ctx)



CRYPTO_BOOL Kdiv(const digit_t *, const digit_t *, digit_t *,
                 const field_desc_t*, digit_t *, struct bigctx_t *f_pBigCtx);
CRYPTO_BOOL Kfree(field_desc_t*, struct bigctx_t *f_pBigCtx);
CRYPTO_BOOL Kimmediate(const sdigit_t, digit_t *,
                       const field_desc_t*, struct bigctx_t *f_pBigCtx);

CRYPTO_BOOL Kinitialize_prime(const mp_modulus_t*, field_desc_t*,
                             struct bigctx_t *,
                             struct bigctx_t *f_pBigCtx);
CRYPTO_BOOL Kinvert(const digit_t *, digit_t *,
                    const field_desc_t*, digit_t *, struct bigctx_t *f_pBigCtx);
CRYPTO_BOOL Kinvert_many(const digit_t *, digit_t *,
                         const CRYPTO_DWORD, const field_desc_t*,
                         digit_t *, struct bigctx_t *f_pBigCtx);

#define Kmul(f1, f2, f3, fdesc, temps, ctx) Kmul_many((f1), (f2),\
                                       (f3), 1, (fdesc), (temps), ctx)

CRYPTO_BOOL Kmul_many(const digit_t *, const digit_t *, digit_t *,
                        const CRYPTO_DWORD, const field_desc_t*,
                        digit_t *, struct bigctx_t *f_pBigCtx);
CRYPTO_BOOL Kmuladd (const digit_t *, const digit_t *,
                        const digit_t *, digit_t *,
                        const field_desc_t*, digit_t *, struct bigctx_t *f_pBigCtx);

CRYPTO_BOOL Kprime_sqrter
        (const digit_t      *f1,        // IN
         digit_t       *f3,        // OUT
         const CRYPTO_DWORD      nelmt,     // IN
         const field_desc_t *fdesc,     // IN
         CRYPTO_BOOL          *psquares,  // OUT
         struct bigctx_t *f_pBigCtx);



#endif /* __FIELD_H */
