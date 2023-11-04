# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
add_subdirectory(src/ibp)
if(IBP_BUILD_TESTS)
    add_subdirectory(test)
endif()
