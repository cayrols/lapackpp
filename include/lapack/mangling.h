#ifndef LAPACK_MANGLING_H
#define LAPACK_MANGLING_H

// -----------------------------------------------------------------------------
// Fortran name mangling depends on compiler.
// Define FORTRAN_UPPER for uppercase,
// define FORTRAN_LOWER for lowercase (IBM xlf),
// else the default is lowercase with appended underscore
// (GNU gcc, Intel icc, PGI pgfortan, Cray ftn).
#ifndef LAPACK_GLOBAL
    #if defined(FORTRAN_UPPER) || defined(LAPACK_GLOBAL_PATTERN_UC)
        #define LAPACK_GLOBAL( lower, UPPER ) UPPER
    #elif defined(FORTRAN_LOWER) || defined(LAPACK_GLOBAL_PATTERN_MC)
        #define LAPACK_GLOBAL( lower, UPPER ) lower
    #else
        #define LAPACK_GLOBAL( lower, UPPER ) lower##_
    #endif
#endif

#endif  /* LAPACK_MANGLING_H */