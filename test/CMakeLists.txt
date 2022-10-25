# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
include(icm_testing)

CPMAddPackage(gh:iboB/doctest-util@0.0.5)

macro(ibp_test test)
    add_doctest_lib_test(${test} ibp ${ARGN})
endmacro()

ibp_test(ChunkedBlockStorage t-ChunkedBlockStorage.cpp)
