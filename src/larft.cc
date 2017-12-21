#include "lapack.hh"
#include "lapack_fortran.h"

#include <vector>

namespace lapack {

using blas::max;
using blas::min;
using blas::real;

// -----------------------------------------------------------------------------
/// @ingroup unitary_computational
void larft(
    lapack::Direct direct, lapack::StoreV storev, int64_t n, int64_t k,
    float const* V, int64_t ldv,
    float const* tau,
    float* T, int64_t ldt )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(blas_int)) {
        throw_if_( std::abs(n) > std::numeric_limits<blas_int>::max() );
        throw_if_( std::abs(k) > std::numeric_limits<blas_int>::max() );
        throw_if_( std::abs(ldv) > std::numeric_limits<blas_int>::max() );
        throw_if_( std::abs(ldt) > std::numeric_limits<blas_int>::max() );
    }
    char direct_ = direct2char( direct );
    char storev_ = storev2char( storev );
    blas_int n_ = (blas_int) n;
    blas_int k_ = (blas_int) k;
    blas_int ldv_ = (blas_int) ldv;
    blas_int ldt_ = (blas_int) ldt;

    LAPACK_slarft( &direct_, &storev_, &n_, &k_, V, &ldv_, tau, T, &ldt_ );
}

// -----------------------------------------------------------------------------
/// @ingroup unitary_computational
void larft(
    lapack::Direct direct, lapack::StoreV storev, int64_t n, int64_t k,
    double const* V, int64_t ldv,
    double const* tau,
    double* T, int64_t ldt )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(blas_int)) {
        throw_if_( std::abs(n) > std::numeric_limits<blas_int>::max() );
        throw_if_( std::abs(k) > std::numeric_limits<blas_int>::max() );
        throw_if_( std::abs(ldv) > std::numeric_limits<blas_int>::max() );
        throw_if_( std::abs(ldt) > std::numeric_limits<blas_int>::max() );
    }
    char direct_ = direct2char( direct );
    char storev_ = storev2char( storev );
    blas_int n_ = (blas_int) n;
    blas_int k_ = (blas_int) k;
    blas_int ldv_ = (blas_int) ldv;
    blas_int ldt_ = (blas_int) ldt;

    LAPACK_dlarft( &direct_, &storev_, &n_, &k_, V, &ldv_, tau, T, &ldt_ );
}

// -----------------------------------------------------------------------------
/// @ingroup unitary_computational
void larft(
    lapack::Direct direct, lapack::StoreV storev, int64_t n, int64_t k,
    std::complex<float> const* V, int64_t ldv,
    std::complex<float> const* tau,
    std::complex<float>* T, int64_t ldt )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(blas_int)) {
        throw_if_( std::abs(n) > std::numeric_limits<blas_int>::max() );
        throw_if_( std::abs(k) > std::numeric_limits<blas_int>::max() );
        throw_if_( std::abs(ldv) > std::numeric_limits<blas_int>::max() );
        throw_if_( std::abs(ldt) > std::numeric_limits<blas_int>::max() );
    }
    char direct_ = direct2char( direct );
    char storev_ = storev2char( storev );
    blas_int n_ = (blas_int) n;
    blas_int k_ = (blas_int) k;
    blas_int ldv_ = (blas_int) ldv;
    blas_int ldt_ = (blas_int) ldt;

    LAPACK_clarft( &direct_, &storev_, &n_, &k_, V, &ldv_, tau, T, &ldt_ );
}

// -----------------------------------------------------------------------------
/// Forms the triangular factor T of a complex block reflector H
/// of order n, which is defined as a product of k elementary reflectors.
///
/// If direct = Forward, \f$ H = H(1) H(2) \dots H(k) \f$ and T is upper triangular;
///
/// If direct = Backward, \f$ H = H(k) \dots H(2) H(1) \f$ and T is lower triangular.
///
/// If storev = Columnwise, the vector which defines the elementary reflector
/// H(i) is stored in the i-th column of the array V, and
///
///     \f[ H = I - V T V^H \f]
///
/// If storev = Rowwise, the vector which defines the elementary reflector
/// H(i) is stored in the i-th row of the array V, and
///
///     \f[ H = I - V^H T V \f]
///
/// Overloaded versions are available for
/// `float`, `double`, `std::complex<float>`, and `std::complex<double>`.
///
/// @param[in] direct
///     Specifies the order in which the elementary reflectors are
///     multiplied to form the block reflector:
///     - lapack::Direct::Forward:  \f$ H = H(1) H(2) \dots H(k) \f$
///     - lapack::Direct::Backward: \f$ H = H(k) \dots H(2) H(1) \f$
///
/// @param[in] storev
///     Specifies how the vectors which define the elementary
///     reflectors are stored (see also Further Details):
///     - lapack::StoreV::Columnwise
///     - lapack::StoreV::Rowwise
///
/// @param[in] n
///     The order of the block reflector H. n >= 0.
///
/// @param[in] k
///     The order of the triangular factor T (= the number of
///     elementary reflectors). k >= 1.
///
/// @param[in] V
///     - If storev = Columnwise, the n-by-k matrix V, stored in an ldv-by-k array;
///     - if storev = Rowwise,    the k-by-n matrix V, stored in an ldv-by-n array.
///     \n
///     See further details.
///
/// @param[in] ldv
///     The leading dimension of the array V.
///     - If storev = Columnwise, ldv >= max(1,n);
///     - if storev = Rowwise,    ldv >= k.
///
/// @param[in] tau
///     The vector tau of length k.
///     tau(i) must contain the scalar factor of the elementary
///     reflector H(i).
///
/// @param[out] T
///     The k-by-k matrix T, stored in an ldt-by-k array.
///     The k-by-k triangular factor T of the block reflector.
///     - If direct = Forward, T is upper triangular;
///     - if direct = Backward, T is lower triangular.
///     \n
///     The rest of the array is not used.
///
/// @param[in] ldt
///     The leading dimension of the array T. ldt >= k.
///
// -----------------------------------------------------------------------------
/// @par Further Details
///
/// The shape of the matrix V and the storage of the vectors which define
/// the H(i) is best illustrated by the following example with n = 5 and
/// k = 3. The elements equal to 1 are not stored.
///
///     direct = Forward and             direct = Forward and
///     storev = Columnwise:             storev = Rowwise:
///
///     V = (  1       )                 V = (  1 v1 v1 v1 v1 )
///         ( v1  1    )                     (     1 v2 v2 v2 )
///         ( v1 v2  1 )                     (        1 v3 v3 )
///         ( v1 v2 v3 )
///         ( v1 v2 v3 )
///
///     direct = Backward and            direct = Backward and
///     storev = Columnwise:             storev = Rowwise:
///
///     V = ( v1 v2 v3 )                 V = ( v1 v1  1       )
///         ( v1 v2 v3 )                     ( v2 v2 v2  1    )
///         (  1 v2 v3 )                     ( v3 v3 v3 v3  1 )
///         (     1 v3 )
///         (        1 )
///
/// @ingroup unitary_computational
void larft(
    lapack::Direct direct, lapack::StoreV storev, int64_t n, int64_t k,
    std::complex<double> const* V, int64_t ldv,
    std::complex<double> const* tau,
    std::complex<double>* T, int64_t ldt )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(blas_int)) {
        throw_if_( std::abs(n) > std::numeric_limits<blas_int>::max() );
        throw_if_( std::abs(k) > std::numeric_limits<blas_int>::max() );
        throw_if_( std::abs(ldv) > std::numeric_limits<blas_int>::max() );
        throw_if_( std::abs(ldt) > std::numeric_limits<blas_int>::max() );
    }
    char direct_ = direct2char( direct );
    char storev_ = storev2char( storev );
    blas_int n_ = (blas_int) n;
    blas_int k_ = (blas_int) k;
    blas_int ldv_ = (blas_int) ldv;
    blas_int ldt_ = (blas_int) ldt;

    LAPACK_zlarft( &direct_, &storev_, &n_, &k_, V, &ldv_, tau, T, &ldt_ );
}

}  // namespace lapack
