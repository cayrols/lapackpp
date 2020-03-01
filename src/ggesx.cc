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
int64_t ggesx(
    lapack::Job jobvsl, lapack::Job jobvsr, lapack::Sort sort,
    lapack_s_select3 select, lapack::Sense sense, int64_t n,
    float* A, int64_t lda,
    float* B, int64_t ldb,
    int64_t* sdim,
    std::complex<float>* alpha,
    float* beta,
    float* VSL, int64_t ldvsl,
    float* VSR, int64_t ldvsr,
    float* rconde,
    float* rcondv )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(lapack_int)) {
        lapack_error_if( std::abs(n) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(lda) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldb) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldvsl) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldvsr) > std::numeric_limits<lapack_int>::max() );
    }
    char jobvsl_ = job2char( jobvsl );
    char jobvsr_ = job2char( jobvsr );
    char sort_ = sort2char( sort );
    char sense_ = sense2char( sense );
    lapack_int n_ = (lapack_int) n;
    lapack_int lda_ = (lapack_int) lda;
    lapack_int ldb_ = (lapack_int) ldb;
    lapack_int sdim_ = (lapack_int) *sdim;
    lapack_int ldvsl_ = (lapack_int) ldvsl;
    lapack_int ldvsr_ = (lapack_int) ldvsr;
    lapack_int info_ = 0;

    // split-complex representation
    std::vector< float > alphar( max( 1, n ) );
    std::vector< float > alphai( max( 1, n ) );

    // query for workspace size
    float qry_work[1];
    lapack_int qry_iwork[1];
    lapack_int qry_bwork[1];
    lapack_int ineg_one = -1;
    LAPACK_sggesx(
        &jobvsl_, &jobvsr_, &sort_,
        select, &sense_, &n_,
        A, &lda_,
        B, &ldb_, &sdim_,
        &alphar[0],
        &alphai[0],
        beta,
        VSL, &ldvsl_,
        VSR, &ldvsr_,
        rconde,
        rcondv,
        qry_work, &ineg_one,
        qry_iwork, &ineg_one,
        qry_bwork, &info_ );
    if (info_ < 0) {
        throw Error();
    }
    lapack_int lwork_ = real(qry_work[0]);
    lapack_int liwork_ = real(qry_iwork[0]);

    // allocate workspace
    std::vector< float > work( lwork_ );
    std::vector< lapack_int > iwork( liwork_ );
    std::vector< lapack_int > bwork( (n) );

    LAPACK_sggesx(
        &jobvsl_, &jobvsr_, &sort_,
        select, &sense_, &n_,
        A, &lda_,
        B, &ldb_, &sdim_,
        &alphar[0],
        &alphai[0],
        beta,
        VSL, &ldvsl_,
        VSR, &ldvsr_,
        rconde,
        rcondv,
        &work[0], &lwork_,
        &iwork[0], &liwork_,
        &bwork[0], &info_ );
    if (info_ < 0) {
        throw Error();
    }
    *sdim = sdim_;
    // merge split-complex representation
    for (int64_t i = 0; i < n; ++i) {
        alpha[i] = std::complex<double>( alphar[i], alphai[i] );
    }
    return info_;
}

// -----------------------------------------------------------------------------
int64_t ggesx(
    lapack::Job jobvsl, lapack::Job jobvsr, lapack::Sort sort,
    lapack_d_select3 select, lapack::Sense sense, int64_t n,
    double* A, int64_t lda,
    double* B, int64_t ldb,
    int64_t* sdim,
    std::complex<double>* alpha,
    double* beta,
    double* VSL, int64_t ldvsl,
    double* VSR, int64_t ldvsr,
    double* rconde,
    double* rcondv )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(lapack_int)) {
        lapack_error_if( std::abs(n) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(lda) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldb) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldvsl) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldvsr) > std::numeric_limits<lapack_int>::max() );
    }
    char jobvsl_ = job2char( jobvsl );
    char jobvsr_ = job2char( jobvsr );
    char sort_ = sort2char( sort );
    char sense_ = sense2char( sense );
    lapack_int n_ = (lapack_int) n;
    lapack_int lda_ = (lapack_int) lda;
    lapack_int ldb_ = (lapack_int) ldb;
    lapack_int sdim_ = (lapack_int) *sdim;
    lapack_int ldvsl_ = (lapack_int) ldvsl;
    lapack_int ldvsr_ = (lapack_int) ldvsr;
    lapack_int info_ = 0;

    // split-complex representation
    std::vector< double > alphar( max( 1, n ) );
    std::vector< double > alphai( max( 1, n ) );

    // query for workspace size
    double qry_work[1];
    lapack_int qry_iwork[1];
    lapack_int qry_bwork[1];
    lapack_int ineg_one = -1;
    LAPACK_dggesx(
        &jobvsl_, &jobvsr_, &sort_,
        select, &sense_, &n_,
        A, &lda_,
        B, &ldb_, &sdim_,
        &alphar[0],
        &alphai[0],
        beta,
        VSL, &ldvsl_,
        VSR, &ldvsr_,
        rconde,
        rcondv,
        qry_work, &ineg_one,
        qry_iwork, &ineg_one,
        qry_bwork, &info_ );
    if (info_ < 0) {
        throw Error();
    }
    lapack_int lwork_ = real(qry_work[0]);
    lapack_int liwork_ = real(qry_iwork[0]);

    // allocate workspace
    std::vector< double > work( lwork_ );
    std::vector< lapack_int > iwork( liwork_ );
    std::vector< lapack_int > bwork( (n) );

    LAPACK_dggesx(
        &jobvsl_, &jobvsr_, &sort_,
        select, &sense_, &n_,
        A, &lda_,
        B, &ldb_, &sdim_,
        &alphar[0],
        &alphai[0],
        beta,
        VSL, &ldvsl_,
        VSR, &ldvsr_,
        rconde,
        rcondv,
        &work[0], &lwork_,
        &iwork[0], &liwork_,
        &bwork[0], &info_ );
    if (info_ < 0) {
        throw Error();
    }
    *sdim = sdim_;
    // merge split-complex representation
    for (int64_t i = 0; i < n; ++i) {
        alpha[i] = std::complex<float>( alphar[i], alphai[i] );
    }
    return info_;
}

// -----------------------------------------------------------------------------
int64_t ggesx(
    lapack::Job jobvsl, lapack::Job jobvsr, lapack::Sort sort,
    lapack_c_select2 select, lapack::Sense sense, int64_t n,
    std::complex<float>* A, int64_t lda,
    std::complex<float>* B, int64_t ldb,
    int64_t* sdim,
    std::complex<float>* alpha,
    std::complex<float>* beta,
    std::complex<float>* VSL, int64_t ldvsl,
    std::complex<float>* VSR, int64_t ldvsr,
    float* rconde,
    float* rcondv )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(lapack_int)) {
        lapack_error_if( std::abs(n) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(lda) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldb) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldvsl) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldvsr) > std::numeric_limits<lapack_int>::max() );
    }
    char jobvsl_ = job2char( jobvsl );
    char jobvsr_ = job2char( jobvsr );
    char sort_ = sort2char( sort );
    char sense_ = sense2char( sense );
    lapack_int n_ = (lapack_int) n;
    lapack_int lda_ = (lapack_int) lda;
    lapack_int ldb_ = (lapack_int) ldb;
    lapack_int sdim_ = (lapack_int) *sdim;
    lapack_int ldvsl_ = (lapack_int) ldvsl;
    lapack_int ldvsr_ = (lapack_int) ldvsr;
    lapack_int info_ = 0;

    // query for workspace size
    std::complex<float> qry_work[1];
    float qry_rwork[1];
    lapack_int qry_iwork[1];
    lapack_int qry_bwork[1];
    lapack_int ineg_one = -1;
    LAPACK_cggesx(
        &jobvsl_, &jobvsr_, &sort_,
        (LAPACK_C_SELECT2) select, &sense_, &n_,
        (lapack_complex_float*) A, &lda_,
        (lapack_complex_float*) B, &ldb_, &sdim_,
        (lapack_complex_float*) alpha,
        (lapack_complex_float*) beta,
        (lapack_complex_float*) VSL, &ldvsl_,
        (lapack_complex_float*) VSR, &ldvsr_,
        rconde,
        rcondv,
        (lapack_complex_float*) qry_work, &ineg_one,
        qry_rwork,
        qry_iwork, &ineg_one,
        qry_bwork, &info_ );
    if (info_ < 0) {
        throw Error();
    }
    lapack_int lwork_ = real(qry_work[0]);
    lapack_int liwork_ = real(qry_iwork[0]);

    // allocate workspace
    std::vector< std::complex<float> > work( lwork_ );
    std::vector< float > rwork( (8*n) );
    std::vector< lapack_int > iwork( liwork_ );
    std::vector< lapack_int > bwork( (n) );

    LAPACK_cggesx(
        &jobvsl_, &jobvsr_, &sort_,
        (LAPACK_C_SELECT2) select, &sense_, &n_,
        (lapack_complex_float*) A, &lda_,
        (lapack_complex_float*) B, &ldb_, &sdim_,
        (lapack_complex_float*) alpha,
        (lapack_complex_float*) beta,
        (lapack_complex_float*) VSL, &ldvsl_,
        (lapack_complex_float*) VSR, &ldvsr_,
        rconde,
        rcondv,
        (lapack_complex_float*) &work[0], &lwork_,
        &rwork[0],
        &iwork[0], &liwork_,
        &bwork[0], &info_ );
    if (info_ < 0) {
        throw Error();
    }
    *sdim = sdim_;
    return info_;
}

// -----------------------------------------------------------------------------
int64_t ggesx(
    lapack::Job jobvsl, lapack::Job jobvsr, lapack::Sort sort,
    lapack_z_select2 select, lapack::Sense sense, int64_t n,
    std::complex<double>* A, int64_t lda,
    std::complex<double>* B, int64_t ldb,
    int64_t* sdim,
    std::complex<double>* alpha,
    std::complex<double>* beta,
    std::complex<double>* VSL, int64_t ldvsl,
    std::complex<double>* VSR, int64_t ldvsr,
    double* rconde,
    double* rcondv )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(lapack_int)) {
        lapack_error_if( std::abs(n) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(lda) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldb) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldvsl) > std::numeric_limits<lapack_int>::max() );
        lapack_error_if( std::abs(ldvsr) > std::numeric_limits<lapack_int>::max() );
    }
    char jobvsl_ = job2char( jobvsl );
    char jobvsr_ = job2char( jobvsr );
    char sort_ = sort2char( sort );
    char sense_ = sense2char( sense );
    lapack_int n_ = (lapack_int) n;
    lapack_int lda_ = (lapack_int) lda;
    lapack_int ldb_ = (lapack_int) ldb;
    lapack_int sdim_ = (lapack_int) *sdim;
    lapack_int ldvsl_ = (lapack_int) ldvsl;
    lapack_int ldvsr_ = (lapack_int) ldvsr;
    lapack_int info_ = 0;

    // query for workspace size
    std::complex<double> qry_work[1];
    double qry_rwork[1];
    lapack_int qry_iwork[1];
    lapack_int qry_bwork[1];
    lapack_int ineg_one = -1;
    LAPACK_zggesx(
        &jobvsl_, &jobvsr_, &sort_,
        (LAPACK_Z_SELECT2) select, &sense_, &n_,
        (lapack_complex_double*) A, &lda_,
        (lapack_complex_double*) B, &ldb_, &sdim_,
        (lapack_complex_double*) alpha,
        (lapack_complex_double*) beta,
        (lapack_complex_double*) VSL, &ldvsl_,
        (lapack_complex_double*) VSR, &ldvsr_,
        rconde,
        rcondv,
        (lapack_complex_double*) qry_work, &ineg_one,
        qry_rwork,
        qry_iwork, &ineg_one,
        qry_bwork, &info_ );
    if (info_ < 0) {
        throw Error();
    }
    lapack_int lwork_ = real(qry_work[0]);
    lapack_int liwork_ = real(qry_iwork[0]);

    // allocate workspace
    std::vector< std::complex<double> > work( lwork_ );
    std::vector< double > rwork( (8*n) );
    std::vector< lapack_int > iwork( liwork_ );
    std::vector< lapack_int > bwork( (n) );

    LAPACK_zggesx(
        &jobvsl_, &jobvsr_, &sort_,
        (LAPACK_Z_SELECT2) select, &sense_, &n_,
        (lapack_complex_double*) A, &lda_,
        (lapack_complex_double*) B, &ldb_, &sdim_,
        (lapack_complex_double*) alpha,
        (lapack_complex_double*) beta,
        (lapack_complex_double*) VSL, &ldvsl_,
        (lapack_complex_double*) VSR, &ldvsr_,
        rconde,
        rcondv,
        (lapack_complex_double*) &work[0], &lwork_,
        &rwork[0],
        &iwork[0], &liwork_,
        &bwork[0], &info_ );
    if (info_ < 0) {
        throw Error();
    }
    *sdim = sdim_;
    return info_;
}

}  // namespace lapack
