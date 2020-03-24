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
/// @ingroup posv
int64_t posvx(
    lapack::Factored fact, lapack::Uplo uplo, int64_t n, int64_t nrhs,
    float* A, int64_t lda,
    float* AF, int64_t ldaf,
    lapack::Equed* equed,
    float* S,
    float* B, int64_t ldb,
    float* X, int64_t ldx,
    float* rcond,
    float* ferr,
    float* berr )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(lapack_int)) {
        lapack_error_if( std::abs(n) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(nrhs) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(lda) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldaf) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldb) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldx) > std::numeric_limits<lapack_int>::max() );
    }
    char fact_ = factored2char( fact );
    char uplo_ = uplo2char( uplo );
    char equed_ = equed2char( *equed );
    lapack_int n_ = (lapack_int) n;
    lapack_int nrhs_ = (lapack_int) nrhs;
    lapack_int lda_ = (lapack_int) lda;
    lapack_int ldaf_ = (lapack_int) ldaf;
    lapack_int ldb_ = (lapack_int) ldb;
    lapack_int ldx_ = (lapack_int) ldx;
    lapack_int info_ = 0;

    // allocate workspace
    std::vector< float > work( (3*n) );
    std::vector< lapack_int > iwork( (n) );

    LAPACK_sposvx(
        &fact_, &uplo_, &n_, &nrhs_,
        A, &lda_,
        AF, &ldaf_,
        &equed_,
        S,
        B, &ldb_,
        X, &ldx_, rcond,
        ferr,
        berr,
        &work[0],
        &iwork[0], &info_ );
    if (info_ < 0) {
        throw Error();
    }
    *equed = char2equed( equed_ );
    return info_;
}

// -----------------------------------------------------------------------------
/// @ingroup posv
int64_t posvx(
    lapack::Factored fact, lapack::Uplo uplo, int64_t n, int64_t nrhs,
    double* A, int64_t lda,
    double* AF, int64_t ldaf,
    lapack::Equed* equed,
    double* S,
    double* B, int64_t ldb,
    double* X, int64_t ldx,
    double* rcond,
    double* ferr,
    double* berr )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(lapack_int)) {
        lapack_error_if( std::abs(n) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(nrhs) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(lda) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldaf) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldb) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldx) > std::numeric_limits<lapack_int>::max() );
    }
    char fact_ = factored2char( fact );
    char uplo_ = uplo2char( uplo );
    char equed_ = equed2char( *equed );
    lapack_int n_ = (lapack_int) n;
    lapack_int nrhs_ = (lapack_int) nrhs;
    lapack_int lda_ = (lapack_int) lda;
    lapack_int ldaf_ = (lapack_int) ldaf;
    lapack_int ldb_ = (lapack_int) ldb;
    lapack_int ldx_ = (lapack_int) ldx;
    lapack_int info_ = 0;

    // allocate workspace
    std::vector< double > work( (3*n) );
    std::vector< lapack_int > iwork( (n) );

    LAPACK_dposvx(
        &fact_, &uplo_, &n_, &nrhs_,
        A, &lda_,
        AF, &ldaf_,
        &equed_,
        S,
        B, &ldb_,
        X, &ldx_, rcond,
        ferr,
        berr,
        &work[0],
        &iwork[0], &info_ );
    if (info_ < 0) {
        throw Error();
    }
    *equed = char2equed( equed_ );
    return info_;
}

// -----------------------------------------------------------------------------
/// @ingroup posv
int64_t posvx(
    lapack::Factored fact, lapack::Uplo uplo, int64_t n, int64_t nrhs,
    std::complex<float>* A, int64_t lda,
    std::complex<float>* AF, int64_t ldaf,
    lapack::Equed* equed,
    float* S,
    std::complex<float>* B, int64_t ldb,
    std::complex<float>* X, int64_t ldx,
    float* rcond,
    float* ferr,
    float* berr )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(lapack_int)) {
        lapack_error_if( std::abs(n) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(nrhs) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(lda) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldaf) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldb) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldx) > std::numeric_limits<lapack_int>::max() );
    }
    char fact_ = factored2char( fact );
    char uplo_ = uplo2char( uplo );
    char equed_ = equed2char( *equed );
    lapack_int n_ = (lapack_int) n;
    lapack_int nrhs_ = (lapack_int) nrhs;
    lapack_int lda_ = (lapack_int) lda;
    lapack_int ldaf_ = (lapack_int) ldaf;
    lapack_int ldb_ = (lapack_int) ldb;
    lapack_int ldx_ = (lapack_int) ldx;
    lapack_int info_ = 0;

    // allocate workspace
    std::vector< std::complex<float> > work( (2*n) );
    std::vector< float > rwork( (n) );

    LAPACK_cposvx(
        &fact_, &uplo_, &n_, &nrhs_,
        (lapack_complex_float*) A, &lda_,
        (lapack_complex_float*) AF, &ldaf_,
        &equed_,
        S,
        (lapack_complex_float*) B, &ldb_,
        (lapack_complex_float*) X, &ldx_, rcond,
        ferr,
        berr,
        (lapack_complex_float*) &work[0],
        &rwork[0], &info_ );
    if (info_ < 0) {
        throw Error();
    }
    *equed = char2equed( equed_ );
    return info_;
}

// -----------------------------------------------------------------------------
/// Uses the Cholesky factorization
///     $A = U^H U$ or
///     $A = L L^H$ to
/// compute the solution to a system of linear equations
/// \[
///     A X = B,
/// \]
/// where A is an n-by-n Hermitian positive definite matrix and X and B
/// are n-by-nrhs matrices.
///
/// Error bounds on the solution and a condition estimate are also
/// provided.
///
/// Overloaded versions are available for
/// `float`, `double`, `std::complex<float>`, and `std::complex<double>`.
///
/// @param[in] fact
///     Whether or not the factored form of the matrix A is
///     supplied on entry, and if not, whether the matrix A should be
///     equilibrated before it is factored.
///     - lapack::Factored::Factored:
///         On entry, AF contains the factored form of A.
///         If equed = Yes, the matrix A has been equilibrated
///         with scaling factors given by S.
///         A and AF will not be modified.
///
///     - lapack::Factored::NotFactored:
///         The matrix A will be copied to AF and factored.
///
///     - lapack::Factored::Equilibrate:
///         The matrix A will be equilibrated if necessary, then
///         copied to AF and factored.
///
/// @param[in] uplo
///     - lapack::Uplo::Upper: Upper triangle of A is stored;
///     - lapack::Uplo::Lower: Lower triangle of A is stored.
///
/// @param[in] n
///     The number of linear equations, i.e., the order of the
///     matrix A. n >= 0.
///
/// @param[in] nrhs
///     The number of right hand sides, i.e., the number of columns
///     of the matrices B and X. nrhs >= 0.
///
/// @param[in,out] A
///     The n-by-n matrix A, stored in an lda-by-n array.
///     On entry, the Hermitian matrix A, except if fact = Factored and
///     equed = Yes, then A must contain the equilibrated matrix
///     $\text{diag}(S) \; A \; \text{diag}(S).$
///     - If uplo = Upper, the leading
///     n-by-n upper triangular part of A contains the upper
///     triangular part of the matrix A, and the strictly lower
///     triangular part of A is not referenced.
///
///     - If uplo = Lower, the
///     leading n-by-n lower triangular part of A contains the lower
///     triangular part of the matrix A, and the strictly upper
///     triangular part of A is not referenced.
///
///     - A is not modified if fact = Factored or NotFactored,
///     or if fact = Equilibrate and equed = None on exit.
///
///     - On exit, if fact = Equilibrate and equed = Yes, A is overwritten by
///     $\text{diag}(S) \; A \; \text{diag}(S).$
///
/// @param[in] lda
///     The leading dimension of the array A. lda >= max(1,n).
///
/// @param[in,out] AF
///     The n-by-n matrix AF, stored in an ldaf-by-n array.
///     - If fact = Factored, then AF is an input argument and on entry
///     contains the triangular factor U or L from the Cholesky
///     factorization $A = U^H U$ or $A = L L^H,$ in the same storage
///     format as A.
///
///     - If equed != None, then AF is the factored form
///     of the equilibrated matrix $\text{diag}(S) \; A \; \text{diag}(S).$
///
///     - If fact = NotFactored, then AF is an output argument and on exit
///     returns the triangular factor U or L from the Cholesky
///     factorization $A = U^H U$ or $A = L L^H$ of the original
///     matrix A.
///
///     - If fact = Equilibrate, then AF is an output argument and on exit
///     returns the triangular factor U or L from the Cholesky
///     factorization $A = U^H U$ or $A = L L^H$ of the equilibrated
///     matrix A (see the description of A for the form of the
///     equilibrated matrix).
///
/// @param[in] ldaf
///     The leading dimension of the array AF. ldaf >= max(1,n).
///
/// @param[in,out] equed
///     The form of equilibration that was done:
///     - lapack::Equed::None:
///         No equilibration (always true if fact = NotFactored).
///     - lapack::Equed::Yes:
///         Equilibration was done, i.e.,
///         A has been replaced by $\text{diag}(S) \; A \; \text{diag}(S).$
///
///     - equed is an input argument if fact = Factored; otherwise, it is an
///     output argument.
///
/// @param[in,out] S
///     The vector S of length n.
///     The scale factors for A.
///     - If fact = Factored, S is an input argument;
///     - otherwise, S is an output argument.
///     - If fact = Factored and equed = Yes, each element of S must be positive.
///     - If equed = None, S is not accessed.
///
/// @param[in,out] B
///     The n-by-nrhs matrix B, stored in an ldb-by-nrhs array.
///     On entry, the n-by-nrhs righthand side matrix B.
///     On exit, if equed = None, B is not modified; if equed = Yes,
///     B is overwritten by $\text{diag}(S) \; B.$
///
/// @param[in] ldb
///     The leading dimension of the array B. ldb >= max(1,n).
///
/// @param[out] X
///     The n-by-nrhs matrix X, stored in an ldx-by-nrhs array.
///     If successful or return value = n+1, the n-by-nrhs solution matrix X to
///     the original system of equations. Note that if equed = Yes,
///     A and B are modified on exit, and the solution to the
///     equilibrated system is $\text{diag}(S)^{-1} X.$
///
/// @param[in] ldx
///     The leading dimension of the array X. ldx >= max(1,n).
///
/// @param[out] rcond
///     The estimate of the reciprocal condition number of the matrix
///     A after equilibration (if done). If rcond is less than the
///     machine precision (in particular, if rcond = 0), the matrix
///     is singular to working precision. This condition is
///     indicated by a return code of return value > 0.
///
/// @param[out] ferr
///     The vector ferr of length nrhs.
///     The estimated forward error bound for each solution vector
///     X(j) (the j-th column of the solution matrix X).
///     If XTRUE is the true solution corresponding to X(j), ferr(j)
///     is an estimated upper bound for the magnitude of the largest
///     element in (X(j) - XTRUE) divided by the magnitude of the
///     largest element in X(j). The estimate is as reliable as
///     the estimate for rcond, and is almost always a slight
///     overestimate of the true error.
///
/// @param[out] berr
///     The vector berr of length nrhs.
///     The componentwise relative backward error of each solution
///     vector X(j) (i.e., the smallest relative change in
///     any element of A or B that makes X(j) an exact solution).
///
/// @retval = 0: successful exit
/// @retval > 0 and <= n: if return value = i,
///     the leading minor of order i of A is
///     not positive definite, so the factorization
///     could not be completed, and the solution has not
///     been computed. rcond = 0 is returned.
/// @retval = n+1: U is nonsingular, but rcond is less than machine
///     precision, meaning that the matrix is singular
///     to working precision. Nevertheless, the
///     solution and error bounds are computed because
///     there are a number of situations where the
///     computed solution can be more accurate than the
///     value of rcond would suggest.
///
/// @ingroup posv
int64_t posvx(
    lapack::Factored fact, lapack::Uplo uplo, int64_t n, int64_t nrhs,
    std::complex<double>* A, int64_t lda,
    std::complex<double>* AF, int64_t ldaf,
    lapack::Equed* equed,
    double* S,
    std::complex<double>* B, int64_t ldb,
    std::complex<double>* X, int64_t ldx,
    double* rcond,
    double* ferr,
    double* berr )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(lapack_int)) {
        lapack_error_if( std::abs(n) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(nrhs) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(lda) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldaf) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldb) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldx) > std::numeric_limits<lapack_int>::max() );
    }
    char fact_ = factored2char( fact );
    char uplo_ = uplo2char( uplo );
    char equed_ = equed2char( *equed );
    lapack_int n_ = (lapack_int) n;
    lapack_int nrhs_ = (lapack_int) nrhs;
    lapack_int lda_ = (lapack_int) lda;
    lapack_int ldaf_ = (lapack_int) ldaf;
    lapack_int ldb_ = (lapack_int) ldb;
    lapack_int ldx_ = (lapack_int) ldx;
    lapack_int info_ = 0;

    // allocate workspace
    std::vector< std::complex<double> > work( (2*n) );
    std::vector< double > rwork( (n) );

    LAPACK_zposvx(
        &fact_, &uplo_, &n_, &nrhs_,
        (lapack_complex_double*) A, &lda_,
        (lapack_complex_double*) AF, &ldaf_,
        &equed_,
        S,
        (lapack_complex_double*) B, &ldb_,
        (lapack_complex_double*) X, &ldx_, rcond,
        ferr,
        berr,
        (lapack_complex_double*) &work[0],
        &rwork[0], &info_ );
    if (info_ < 0) {
        throw Error();
    }
    *equed = char2equed( equed_ );
    return info_;
}

}  // namespace lapack
