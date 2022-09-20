#!/usr/bin/env python
#
# Copyright (c) 2017-2022, University of Tennessee. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause
# This program is free software: you can redistribute it and/or modify it under
# the terms of the BSD 3-Clause license. See the accompanying LICENSE file.
#
# Usage: python configure.py [--interactive]

from __future__ import print_function

import sys
import re
import config
from   config import Error, font, print_warn, print_header
import config.lapack

#-------------------------------------------------------------------------------
# header

print( '-'*80 + '\n' +
font.bold( font.blue( '                              Welcome to LAPACK++.' ) ) +
'''

By default, configure will automatically choose the first valid value it finds
for each option. You can set it to interactive to find all possible values and
give you a choice:
    ''' + font.blue( 'make config interactive=1' ) + '''

If you have multiple compilers, we suggest specifying your desired compiler by
setting CXX, as the automated search may prefer a different compiler.

For options, see the `INSTALL.md` file.

Configure assumes environment variables CPATH, LIBRARY_PATH, and LD_LIBRARY_PATH
are set so your compiler can find libraries. See INSTALL.md for more details.
''' + '-'*80 )

#-------------------------------------------------------------------------------
def main():
    config.init( namespace='LAPACK', prefix='/opt/slate' )
    config.prog_cxx()

    print_header( 'C++ compiler flags' )
    # Pick highest level supported. oneAPI needs C++17.
    config.prog_cxx_flag(
        ['-std=c++20', '-std=c++17', '-std=c++14', '-std=c++11'])
    config.prog_cxx_flag( '-O2' )
    config.prog_cxx_flag( '-MMD' )
    config.prog_cxx_flag( '-Wall' )
    config.prog_cxx_flag( '-Wno-unused-local-typedefs' )
    config.prog_cxx_flag( '-Wno-unused-function' )
   #config.prog_cxx_flag( '-pedantic',  # todo: conflict with ROCm 3.9.0
   #config.prog_cxx_flag( '-Wshadow',   # todo: conflict with ROCm 3.9.0
   #config.prog_cxx_flag( '-Wmissing-declarations' )
   #config.prog_cxx_flag( '-Wconversion' )
   #config.prog_cxx_flag( '-Werror' )

    config.openmp()

    config.lapack.blas()
    print()
    config.lapack.blas_float_return()
    config.lapack.blas_complex_return()
    config.lapack.vendor_version()

    # Must test mkl_version before cblas and lapacke, to define HAVE_MKL.
    try:
        config.lapack.cblas()
    except Error:
        print_warn( 'LAPACK++ needs CBLAS for testers.' )

    config.lapack.lapack()
    config.lapack.lapack_version()

    # XBLAS and Matgen are optional
    try:
        config.lapack.lapack_xblas()
    except Error:
        print_warn( 'LAPACK++ will exclude wrappers for XBLAS.' )

    try:
        config.lapack.lapack_matgen()
    except Error:
        print_warn( 'LAPACK++ will exclude wrappers for matgen.' )

    try:
        config.lapack.lapacke()
    except Error:
        print_warn( 'LAPACK++ needs LAPACKE for testers.' )

    config.gpu_blas()

    blaspp = config.get_package(
        'BLAS++',
        ['../blaspp', './blaspp'],
        'include/blas.hh',
        'https://github.com/icl-utk-edu/blaspp',
        'https://github.com/icl-utk-edu/blaspp/tarball/master',
        'blaspp.tar.gz' )
    if (not blaspp):
        raise Exception( 'LAPACK++ requires BLAS++.' )

    testsweeper = config.get_package(
        'TestSweeper',
        ['../testsweeper', blaspp + '/testsweeper', './testsweeper'],
        'testsweeper.hh',
        'https://github.com/icl-utk-edu/testsweeper',
        'https://github.com/icl-utk-edu/testsweeper/tarball/master',
        'testsweeper.tar.gz' )
    if (not testsweeper):
        print_warn( 'LAPACK++ needs TestSweeper for testers.' )

    config.extract_defines_from_flags( 'CXXFLAGS', 'lapackpp_header_defines' )
    config.output_files( ['make.inc', 'include/lapack/defines.h'] )
    print( 'log in config/log.txt' )

    print( '-'*80 )
# end

#-------------------------------------------------------------------------------
try:
    main()
except Error as ex:
    print_warn( 'A fatal error occurred. ' + str(ex) +
                '\nLAPACK++ could not be configured. Log in config/log.txt' )
    exit(1)
