// Copyright (c) 2017-2020, University of Tennessee. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause
// This program is free software: you can redistribute it and/or modify it under
// the terms of the BSD 3-Clause license. See the accompanying LICENSE file.

#include "lapack.hh"
#include "lapack/fortran.h"

#include <vector>

namespace lapack {

using blas::max;
using blas::min;
using blas::real;

// -----------------------------------------------------------------------------
/// @ingroup unitary_computational
void larfx(
    lapack::Side side, int64_t m, int64_t n,
    float const* v, float tau,
    float* C, int64_t ldc )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(lapack_int)) {
        lapack_error_if( std::abs(m) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(n) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldc) > std::numeric_limits<lapack_int>::max() );
    }
    char side_ = side2char( side );
    lapack_int m_ = (lapack_int) m;
    lapack_int n_ = (lapack_int) n;
    lapack_int ldc_ = (lapack_int) ldc;

    // from docs
    int64_t lwork = (side == Side::Left ? n : m);

    // allocate workspace
    std::vector< float > work( lwork );

    LAPACK_slarfx(
        &side_, &m_, &n_,
        v, &tau,
        C, &ldc_,
        &work[0] );
}

// -----------------------------------------------------------------------------
/// @ingroup unitary_computational
void larfx(
    lapack::Side side, int64_t m, int64_t n,
    double const* v, double tau,
    double* C, int64_t ldc )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(lapack_int)) {
        lapack_error_if( std::abs(m) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(n) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldc) > std::numeric_limits<lapack_int>::max() );
    }
    char side_ = side2char( side );
    lapack_int m_ = (lapack_int) m;
    lapack_int n_ = (lapack_int) n;
    lapack_int ldc_ = (lapack_int) ldc;

    // from docs
    int64_t lwork = (side == Side::Left ? n : m);

    // allocate workspace
    std::vector< double > work( lwork );

    LAPACK_dlarfx(
        &side_, &m_, &n_,
        v, &tau,
        C, &ldc_,
        &work[0] );
}

// -----------------------------------------------------------------------------
/// @ingroup unitary_computational
void larfx(
    lapack::Side side, int64_t m, int64_t n,
    std::complex<float> const* v, std::complex<float> tau,
    std::complex<float>* C, int64_t ldc )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(lapack_int)) {
        lapack_error_if( std::abs(m) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(n) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldc) > std::numeric_limits<lapack_int>::max() );
    }
    char side_ = side2char( side );
    lapack_int m_ = (lapack_int) m;
    lapack_int n_ = (lapack_int) n;
    lapack_int ldc_ = (lapack_int) ldc;

    // from docs
    int64_t lwork = (side == Side::Left ? n : m);

    // allocate workspace
    std::vector< std::complex<float> > work( lwork );

    LAPACK_clarfx(
        &side_, &m_, &n_,
        (lapack_complex_float*) v, (lapack_complex_float*) &tau,
        (lapack_complex_float*) C, &ldc_,
        (lapack_complex_float*) &work[0] );
}

// -----------------------------------------------------------------------------
/// Applies a elementary reflector H to a m-by-n
/// matrix C, from either the left or the right. H is represented in the
/// form
///
///     \f[ H = I - \tau v v^H \f]
///
/// where \f$ \tau \f$ is a scalar and v is a vector.
///
/// If \f$ \tau = 0, \f$ then H is taken to be the unit matrix
///
/// This version uses inline code if H has order < 11.
///
/// Overloaded versions are available for
/// `float`, `double`, `std::complex<float>`, and `std::complex<double>`.
///
/// @param[in] side
///     - lapack::Side::Left:  form \f$ H C \f$
///     - lapack::Side::Right: form \f$ C H \f$
///
/// @param[in] m
///     The number of rows of the matrix C.
///
/// @param[in] n
///     The number of columns of the matrix C.
///
/// @param[in] v
///     - If side = Left,  the vector v of length m;
///     - if side = right, the vector v of length n.
///
/// @param[in] tau
///     The value tau in the representation of H.
///
/// @param[in,out] C
///     The m-by-n matrix C, stored in an ldc-by-n array.
///     On entry, the m-by-n matrix C.
///     On exit, C is overwritten by the matrix \f$ H C \f$ if side = Left,
///     or \f$ C H \f$ if side = Right.
///
/// @param[in] ldc
///     The leading dimension of the array C. ldc >= max(1,m).
///
/// @ingroup unitary_computational
void larfx(
    lapack::Side side, int64_t m, int64_t n,
    std::complex<double> const* v, std::complex<double> tau,
    std::complex<double>* C, int64_t ldc )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(lapack_int)) {
        lapack_error_if( std::abs(m) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(n) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldc) > std::numeric_limits<lapack_int>::max() );
    }
    char side_ = side2char( side );
    lapack_int m_ = (lapack_int) m;
    lapack_int n_ = (lapack_int) n;
    lapack_int ldc_ = (lapack_int) ldc;

    // from docs
    int64_t lwork = (side == Side::Left ? n : m);

    // allocate workspace
    std::vector< std::complex<double> > work( lwork );

    LAPACK_zlarfx(
        &side_, &m_, &n_,
        (lapack_complex_double*) v, (lapack_complex_double*) &tau,
        (lapack_complex_double*) C, &ldc_,
        (lapack_complex_double*) &work[0] );
}

}  // namespace lapack
