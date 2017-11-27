#include "test.hh"
#include "lapack.hh"
#include "lapack_flops.hh"
#include "print_matrix.hh"
#include "error.hh"

#include <vector>
#include <omp.h>
#include <lapacke.h>

// -----------------------------------------------------------------------------
// simple overloaded wrappers around LAPACKE
static lapack_int LAPACKE_lansy(
    char norm, char uplo, lapack_int n, float* A, lapack_int lda )
{
    return LAPACKE_slansy( LAPACK_COL_MAJOR, norm, uplo, n, A, lda );
}

static lapack_int LAPACKE_lansy(
    char norm, char uplo, lapack_int n, double* A, lapack_int lda )
{
    return LAPACKE_dlansy( LAPACK_COL_MAJOR, norm, uplo, n, A, lda );
}

static lapack_int LAPACKE_lansy(
    char norm, char uplo, lapack_int n, std::complex<float>* A, lapack_int lda )
{
    return LAPACKE_clansy( LAPACK_COL_MAJOR, norm, uplo, n, A, lda );
}

static lapack_int LAPACKE_lansy(
    char norm, char uplo, lapack_int n, std::complex<double>* A, lapack_int lda )
{
    return LAPACKE_zlansy( LAPACK_COL_MAJOR, norm, uplo, n, A, lda );
}

// -----------------------------------------------------------------------------
template< typename scalar_t >
void test_lansy_work( Params& params, bool run )
{
    using namespace blas;
    typedef typename traits< scalar_t >::real_t real_t;
    typedef long long lld;

    // get & mark input values
    lapack::Norm norm = params.norm.value();
    lapack::Uplo uplo = params.uplo.value();
    int64_t n = params.dim.n();
    int64_t align = params.align.value();

    // mark non-standard output values
    params.ref_time.value();
    params.ref_gflops.value();

    if (! run)
        return;

    // ---------- setup
    int64_t lda = roundup( max( n, 1 ), align );
    size_t size_A = (size_t) lda * n;

    std::vector< scalar_t > A( size_A );

    int64_t idist = 1;
    int64_t iseed[4] = { 0, 1, 2, 3 };
    lapack::larnv( idist, iseed, A.size(), &A[0] );

    // ---------- run test
    libtest::flush_cache( params.cache.value() );
    double time = omp_get_wtime();
    int64_t info_tst = lapack::lansy( norm, uplo, n, &A[0], lda );
    time = omp_get_wtime() - time;
    if (info_tst != 0) {
        fprintf( stderr, "lapack::lansy returned error %lld\n", (lld) info_tst );
    }

    double gflop = lapack::Gflop< scalar_t >::lansy( norm, n );
    params.time.value()   = time;
    params.gflops.value() = gflop / time;

    if (params.ref.value() == 'y' || params.check.value() == 'y') {
        // ---------- run reference
        libtest::flush_cache( params.cache.value() );
        time = omp_get_wtime();
        int64_t info_ref = LAPACKE_lansy( norm2char(norm), uplo2char(uplo), n, &A[0], lda );
        time = omp_get_wtime() - time;
        if (info_ref != 0) {
            fprintf( stderr, "LAPACKE_lansy returned error %lld\n", (lld) info_ref );
        }

        params.ref_time.value()   = time;
        params.ref_gflops.value() = gflop / time;

        // ---------- check error compared to reference
        real_t error = 0;
        if (info_tst != info_ref) {
            error = 1;
        }
        params.error.value() = error;
        params.okay.value() = (error == 0);  // expect lapackpp == lapacke
    }
}

// -----------------------------------------------------------------------------
void test_lansy( Params& params, bool run )
{
    switch (params.datatype.value()) {
        case libtest::DataType::Integer:
            throw std::exception();
            break;

        case libtest::DataType::Single:
            test_lansy_work< float >( params, run );
            break;

        case libtest::DataType::Double:
            test_lansy_work< double >( params, run );
            break;

        case libtest::DataType::SingleComplex:
            test_lansy_work< std::complex<float> >( params, run );
            break;

        case libtest::DataType::DoubleComplex:
            test_lansy_work< std::complex<double> >( params, run );
            break;
    }
}
