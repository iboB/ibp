# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
icm_add_lib(ibp-cvt-tree IBP_CVT_TREE)
add_library(ibp::cvt-tree ALIAS ibp-cvt-tree)
target_sources(ibp-cvt-tree PRIVATE
    DumpToTree.hpp
    DumpToTree.cpp
)
target_include_directories(ibp-cvt-tree INTERFACE ..)

ibp_dep(splat)
ibp_dep(itlib)

target_link_libraries(ibp-cvt-tree
    PUBLIC
        ibp::core
        splat::splat
        itlib::itlib
)
