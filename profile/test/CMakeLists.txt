# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
ibp_dep(testing)

macro(ibp_profile_test test)
    add_doctest_lib_test(${test} ibp-profile ${ARGN})
endmacro()

ibp_profile_test(ChunkedBlockStorage t-ChunkedBlockStorage.cpp)
