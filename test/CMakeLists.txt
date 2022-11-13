# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
include(icm_testing)

ibp_dep(testing)

macro(ibp_test test)
    add_doctest_lib_test(${test} ibp ${ARGN})
endmacro()

ibp_test(ChunkedBlockStorage t-ChunkedBlockStorage.cpp)
