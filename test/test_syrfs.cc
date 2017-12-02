#include "test.hh"
#include "lapack.hh"
#include "lapack_flops.hh"
#include "print_matrix.hh"
#include "error.hh"

#include <vector>
#include <omp.h>

// -----------------------------------------------------------------------------
// simple overloaded wrappers around LAPACKE
static lapack_int LAPACKE_syrfs(
    char uplo, lapack_int n, lapack_int nrhs, float* A, lapack_int lda, float* AF, lapack_int ldaf, lapack_int* ipiv, float* B, lapack_int ldb, float* X, lapack_int ldx, float* ferr, float* berr )
{
    return LAPACKE_ssyrfs( LAPACK_COL_MAJOR, uplo, n, nrhs, A, lda, AF, ldaf, ipiv, B, ldb, X, ldx, ferr, berr );
}

static lapack_int LAPACKE_syrfs(
    char uplo, lapack_int n, lapack_int nrhs, double* A, lapack_int lda, double* AF, lapack_int ldaf, lapack_int* ipiv, double* B, lapack_int ldb, double* X, lapack_int ldx, double* ferr, double* berr )
{
    return LAPACKE_dsyrfs( LAPACK_COL_MAJOR, uplo, n, nrhs, A, lda, AF, ldaf, ipiv, B, ldb, X, ldx, ferr, berr );
}

static lapack_int LAPACKE_syrfs(
    char uplo, lapack_int n, lapack_int nrhs, std::complex<float>* A, lapack_int lda, std::complex<float>* AF, lapack_int ldaf, lapack_int* ipiv, std::complex<float>* B, lapack_int ldb, std::complex<float>* X, lapack_int ldx, float* ferr, float* berr )
{
    return LAPACKE_csyrfs( LAPACK_COL_MAJOR, uplo, n, nrhs, A, lda, AF, ldaf, ipiv, B, ldb, X, ldx, ferr, berr );
}

static lapack_int LAPACKE_syrfs(
    char uplo, lapack_int n, lapack_int nrhs, std::complex<double>* A, lapack_int lda, std::complex<double>* AF, lapack_int ldaf, lapack_int* ipiv, std::complex<double>* B, lapack_int ldb, std::complex<double>* X, lapack_int ldx, double* ferr, double* berr )
{
    return LAPACKE_zsyrfs( LAPACK_COL_MAJOR, uplo, n, nrhs, A, lda, AF, ldaf, ipiv, B, ldb, X, ldx, ferr, berr );
}

// -----------------------------------------------------------------------------
template< typename scalar_t >
void test_syrfs_work( Params& params, bool run )
{
    using namespace blas;
    typedef typename traits< scalar_t >::real_t real_t;
    typedef long long lld;

    // get & mark input values
    lapack::Uplo uplo = params.uplo.value();
    int64_t n = params.dim.n();
    int64_t nrhs = params.nrhs.value();
    int64_t align = params.align.value();

    // mark non-standard output values
    params.ref_time.value();
    // params.ref_gflops.value();
    // params.gflops.value();

    if (! run)
        return;

    // ---------- setup
    int64_t lda = roundup( max( 1, n ), align );
    int64_t ldaf = roundup( max( 1, n ), align );
    int64_t ldb = roundup( max( 1, n ), align );
    int64_t ldx = roundup( max( 1, n ), align );
    size_t size_A = (size_t) lda * n;
    size_t size_AF = (size_t) ldaf * n;
    size_t size_ipiv = (size_t) (n);
    size_t size_B = (size_t) ldb * nrhs;
    size_t size_X = (size_t) ldx * nrhs;
    size_t size_ferr = (size_t) (nrhs);
    size_t size_berr = (size_t) (nrhs);

    std::vector< scalar_t > A( size_A );
    std::vector< scalar_t > AF( size_AF );
    std::vector< int64_t > ipiv_tst( size_ipiv );
    std::vector< lapack_int > ipiv_ref( size_ipiv );
    std::vector< scalar_t > B( size_B );
    std::vector< scalar_t > X_tst( size_X );
    std::vector< scalar_t > X_ref( size_X );
    std::vector< real_t > ferr_tst( size_ferr );
    std::vector< real_t > ferr_ref( size_ferr );
    std::vector< real_t > berr_tst( size_berr );
    std::vector< real_t > berr_ref( size_berr );

    int64_t idist = 1;
    int64_t iseed[4] = { 0, 1, 2, 3 };
    lapack::larnv( idist, iseed, A.size(), &A[0] );
    lapack::larnv( idist, iseed, AF.size(), &AF[0] );
    lapack::larnv( idist, iseed, B.size(), &B[0] );
    lapack::larnv( idist, iseed, X_tst.size(), &X_tst[0] );
    X_ref = X_tst;

    // ---------- preserve A before factoring for use in syrfs
    AF = A;
    // ---------- factor before test
    int64_t info_factor = lapack::sytrf( uplo, n, &AF[0], lda, &ipiv_tst[0] );
    if (info_factor != 0) {
        fprintf( stderr, "lapack::sytrf returned error %lld\n", (lld) info_factor );
    }
    // ---------- solve before test
    int64_t info_solve = lapack::sytrs( uplo, n, nrhs, &AF[0], lda, &ipiv_tst[0], &B[0], ldb );
    if (info_solve != 0) {
        fprintf( stderr, "lapack::sytrs returned error %lld\n", (lld) info_solve );
    }
    // ---------- run test
    libtest::flush_cache( params.cache.value() );
    double time = omp_get_wtime();
    int64_t info_tst = lapack::syrfs( uplo, n, nrhs, &A[0], lda, &AF[0], ldaf, &ipiv_tst[0], &B[0], ldb, &X_tst[0], ldx, &ferr_tst[0], &berr_tst[0] );
    time = omp_get_wtime() - time;
    if (info_tst != 0) {
        fprintf( stderr, "lapack::syrfs returned error %lld\n", (lld) info_tst );
    }

    // double gflop = lapack::Gflop< scalar_t >::syrfs( n, nrhs );
    params.time.value()   = time;
    // params.gflops.value() = gflop / time;

    if (params.ref.value() == 'y' || params.check.value() == 'y') {
        // ---------- reuse factorization and initialize ipiv_ref
        std::copy( ipiv_tst.begin(), ipiv_tst.end(), ipiv_ref.begin() );
        // ---------- run reference
        libtest::flush_cache( params.cache.value() );
        time = omp_get_wtime();
        int64_t info_ref = LAPACKE_syrfs( uplo2char(uplo), n, nrhs, &A[0], lda, &AF[0], ldaf, &ipiv_ref[0], &B[0], ldb, &X_ref[0], ldx, &ferr_ref[0], &berr_ref[0] );
        time = omp_get_wtime() - time;
        if (info_ref != 0) {
            fprintf( stderr, "LAPACKE_syrfs returned error %lld\n", (lld) info_ref );
        }

        params.ref_time.value()   = time;
        // params.ref_gflops.value() = gflop / time;

        // ---------- check error compared to reference
        real_t error = 0;
        if (info_tst != info_ref) {
            error = 1;
        }
        error += abs_error( X_tst, X_ref );
        error += abs_error( ferr_tst, ferr_ref );
        error += abs_error( berr_tst, berr_ref );
        params.error.value() = error;
        params.okay.value() = (error == 0);  // expect lapackpp == lapacke
    }
}

// -----------------------------------------------------------------------------
void test_syrfs( Params& params, bool run )
{
    switch (params.datatype.value()) {
        case libtest::DataType::Integer:
            throw std::exception();
            break;

        case libtest::DataType::Single:
            test_syrfs_work< float >( params, run );
            break;

        case libtest::DataType::Double:
            test_syrfs_work< double >( params, run );
            break;

        case libtest::DataType::SingleComplex:
            test_syrfs_work< std::complex<float> >( params, run );
            break;

        case libtest::DataType::DoubleComplex:
            test_syrfs_work< std::complex<double> >( params, run );
            break;
    }
}
