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
void larfg(
    int64_t n,
    float* alpha,
    float* X, int64_t incx,
    float* tau )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(lapack_int)) {
        lapack_error_if( std::abs(n) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(incx) > std::numeric_limits<lapack_int>::max() );
    }
    lapack_int n_ = (lapack_int) n;
    lapack_int incx_ = (lapack_int) incx;

    LAPACK_slarfg(
        &n_, alpha,
        X, &incx_, tau );
}

// -----------------------------------------------------------------------------
/// @ingroup unitary_computational
void larfg(
    int64_t n,
    double* alpha,
    double* X, int64_t incx,
    double* tau )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(lapack_int)) {
        lapack_error_if( std::abs(n) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(incx) > std::numeric_limits<lapack_int>::max() );
    }
    lapack_int n_ = (lapack_int) n;
    lapack_int incx_ = (lapack_int) incx;

    LAPACK_dlarfg(
        &n_, alpha,
        X, &incx_, tau );
}

// -----------------------------------------------------------------------------
/// @ingroup unitary_computational
void larfg(
    int64_t n,
    std::complex<float>* alpha,
    std::complex<float>* X, int64_t incx,
    std::complex<float>* tau )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(lapack_int)) {
        lapack_error_if( std::abs(n) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(incx) > std::numeric_limits<lapack_int>::max() );
    }
    lapack_int n_ = (lapack_int) n;
    lapack_int incx_ = (lapack_int) incx;

    LAPACK_clarfg(
        &n_, (lapack_complex_float*) alpha,
        (lapack_complex_float*) X, &incx_, (lapack_complex_float*) tau );
}

// -----------------------------------------------------------------------------
/// Generates a elementary reflector H of order n, such that:
/**
    \[
        H^H
        \left[ \begin{array}{c}
            \alpha  \\
            x       \\
        \end{array} \right]
        =
        \left[ \begin{array}{c}
            \beta   \\
            0       \\
        \end{array} \right];
        \quad
        H^H H = I.
    \]
*/
/// where $\alpha$ and $\beta$ are scalars, with $\beta$ real,
/// and x is an (n-1)-element vector. H is represented in the form
/**
    \[
        H = I - \tau
        \left[ \begin{array}{c}
            1  \\
            v  \\
        \end{array} \right]
        \left[ \begin{array}{rc}
            1  &  v^H
        \end{array} \right],
    \]
*/
/// where $\tau$ is a scalar and v is a (n-1)-element
/// vector. For complex H, note that H is not hermitian.
///
/// If the elements of x are all zero and alpha is real, then $\tau = 0$
/// and H is taken to be the unit matrix.
///
/// Otherwise $1 \le \text{real}(\tau) \le 2$ and $|\tau - 1| \le 1.$
///
/// Overloaded versions are available for
/// `float`, `double`, `std::complex<float>`, and `std::complex<double>`.
///
/// @param[in] n
///     The order of the elementary reflector.
///
/// @param[in,out] alpha
///     On entry, the value alpha.
///     On exit, it is overwritten with the value beta.
///
/// @param[in,out] X
///     The vector X of length 1+(n-2)*abs(incx).
///     On entry, the vector x.
///     On exit, it is overwritten with the vector v.
///
/// @param[in] incx
///     The increment between elements of X. incx > 0.
///
/// @param[out] tau
///     The value tau.
///
/// @ingroup unitary_computational
void larfg(
    int64_t n,
    std::complex<double>* alpha,
    std::complex<double>* X, int64_t incx,
    std::complex<double>* tau )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(lapack_int)) {
        lapack_error_if( std::abs(n) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(incx) > std::numeric_limits<lapack_int>::max() );
    }
    lapack_int n_ = (lapack_int) n;
    lapack_int incx_ = (lapack_int) incx;

    LAPACK_zlarfg(
        &n_, (lapack_complex_double*) alpha,
        (lapack_complex_double*) X, &incx_, (lapack_complex_double*) tau );
}

}  // namespace lapack
