# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
add_library(ibp-core INTERFACE)
add_iibrary(ibp::core ALIAS ibp-core)
target_include_directories(ibp-core INTERFACE .)

ibp_dep(itlib)
target_link_libraries(ibp-core INTERFACE itlib)
