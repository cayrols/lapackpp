// Copyright (c) 2017-2022, University of Tennessee. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause
// This program is free software: you can redistribute it and/or modify it under
// the terms of the BSD 3-Clause license. See the accompanying LICENSE file.

#include "test.hh"
#include "lapack.hh"
#include "lapack/flops.hh"
#include "print_matrix.hh"
#include "error.hh"
#include "lapacke_wrappers.hh"

#include <vector>

// -----------------------------------------------------------------------------
template< typename scalar_t >
void test_getrf_work( Params& params, bool run )
{
    using real_t = blas::real_type< scalar_t >;
    typedef long long lld;

    // get & mark input values
    int64_t m = params.dim.m();
    int64_t n = params.dim.n();
    int64_t align = params.align();
    int64_t verbose = params.verbose();
    params.matrix.mark();

    real_t eps = std::numeric_limits< real_t >::epsilon();
    real_t tol = params.tol() * eps;

    // mark non-standard output values
    params.ref_time();
    params.ref_gflops();
    params.gflops();

    if (! run)
        return;

    // ---------- setup
    int64_t lda = roundup( blas::max( 1, m ), align );
    size_t size_A = (size_t) lda * n;
    size_t size_ipiv = (size_t) (blas::min(m,n));

    std::vector< scalar_t > A_tst( size_A );
    std::vector< scalar_t > A_ref( size_A );
    std::vector< int64_t > ipiv_tst( size_ipiv );
    std::vector< lapack_int > ipiv_ref( size_ipiv );

    lapack::generate_matrix( params.matrix, m, n, &A_tst[0], lda );
    A_ref = A_tst;

    if (verbose >= 1) {
        printf( "\n"
                "A m=%5lld, n=%5lld, lda=%5lld\n",
                (lld) m, (lld) n, (lld) lda );
    }
    if (verbose >= 2) {
        printf( "A = " ); print_matrix( m, n, &A_tst[0], lda );
    }

    // test error exits
    if (params.error_exit() == 'y') {
        assert_throw( lapack::getrf( -1,  n, &A_tst[0], lda, &ipiv_tst[0] ), lapack::Error );
        assert_throw( lapack::getrf(  m, -1, &A_tst[0], lda, &ipiv_tst[0] ), lapack::Error );
        assert_throw( lapack::getrf(  m,  n, &A_tst[0], m-1, &ipiv_tst[0] ), lapack::Error );
    }

    // ---------- run test
    testsweeper::flush_cache( params.cache() );
    double time = testsweeper::get_wtime();
    int64_t info_tst = lapack::getrf( m, n, &A_tst[0], lda, &ipiv_tst[0] );
    time = testsweeper::get_wtime() - time;
    if (info_tst != 0) {
        fprintf( stderr, "lapack::getrf returned error %lld\n", (lld) info_tst );
    }

    params.time() = time;
    double gflop = lapack::Gflop< scalar_t >::getrf( m, n );
    params.gflops() = gflop / time;

    if (verbose >= 2) {
        printf( "A_factor = " ); print_matrix( m, n, &A_tst[0], lda );
    }

    if (params.check() == 'y' && m == n) {
        // ---------- check error
        // Relative backwards error = ||b - Ax|| / (n * ||A|| * ||x||).
        // For m != n, could check PA - LU.
        int64_t nrhs = 1;
        int64_t ldb = roundup( blas::max( 1, n ), align );
        size_t size_B = (size_t) ldb * nrhs;
        std::vector< scalar_t > B_tst( size_B );
        std::vector< scalar_t > B_ref( size_B );
        int64_t idist = 1;
        int64_t iseed[4] = { 0, 1, 2, 3 };
        lapack::larnv( idist, iseed, B_tst.size(), &B_tst[0] );
        B_ref = B_tst;

        info_tst = lapack::getrs(
            lapack::Op::NoTrans, n, nrhs, &A_tst[0], lda, &ipiv_tst[0], &B_tst[0], ldb );
        if (info_tst != 0) {
            fprintf( stderr, "lapack::getrs returned error %lld\n", (lld) info_tst );
        }

        blas::gemm( blas::Layout::ColMajor, blas::Op::NoTrans, blas::Op::NoTrans,
                    n, nrhs, n,
                    -1.0, &A_ref[0], lda,
                          &B_tst[0], ldb,
                     1.0, &B_ref[0], ldb );
        if (verbose >= 2) {
            printf( "R = " ); print_matrix( n, nrhs, &B_ref[0], ldb );
        }

        real_t error = lapack::lange( lapack::Norm::One, n, nrhs, &B_ref[0], ldb );
        real_t Xnorm = lapack::lange( lapack::Norm::One, n, nrhs, &B_tst[0], ldb );
        real_t Anorm = lapack::lange( lapack::Norm::One, n, n,    &A_ref[0], lda );
        error /= (n * Anorm * Xnorm);
        params.error() = error;
        params.okay() = (error < tol);
    }

    if (params.ref() == 'y') {
        // ---------- run reference
        testsweeper::flush_cache( params.cache() );
        time = testsweeper::get_wtime();
        int64_t info_ref = LAPACKE_getrf( m, n, &A_ref[0], lda, &ipiv_ref[0] );
        time = testsweeper::get_wtime() - time;
        if (info_ref != 0) {
            fprintf( stderr, "LAPACKE_getrf returned error %lld\n", (lld) info_ref );
        }

        params.ref_time() = time;
        params.ref_gflops() = gflop / time;

        if (verbose >= 2) {
            printf( "Aref_factor = " ); print_matrix( m, n, &A_ref[0], lda );
        }
    }
}

// -----------------------------------------------------------------------------
void test_getrf( Params& params, bool run )
{
    switch (params.datatype()) {
        case testsweeper::DataType::Integer:
            throw std::exception();
            break;

        case testsweeper::DataType::Single:
            test_getrf_work< float >( params, run );
            break;

        case testsweeper::DataType::Double:
            test_getrf_work< double >( params, run );
            break;

        case testsweeper::DataType::SingleComplex:
            test_getrf_work< std::complex<float> >( params, run );
            break;

        case testsweeper::DataType::DoubleComplex:
            test_getrf_work< std::complex<double> >( params, run );
            break;
    }
}
