# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
ibp_dep(testing)

macro(ibp_core_test test)
    add_doctest_lib_test(${test} ibp-core ${ARGN})
endmacro()

ibp_core_test(StringBuf t-StringBuf.cpp)
