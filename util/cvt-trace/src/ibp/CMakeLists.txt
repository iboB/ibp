# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
icm_add_lib(ibp-cvt-trace IBP_CVT_TRACE)
add_library(ibp::cvt-trace ALIAS ibp-cvt-trace)
target_sources(ibp-cvt-trace PRIVATE
    DumpToTrace.hpp
    DumpToTrace.cpp
)
target_include_directories(ibp-cvt-trace INTERFACE ..)

ibp_dep(splat)
ibp_dep(jout)

target_link_libraries(ibp-cvt-trace
    PUBLIC
        ibp::core
        splat::splat
    PRIVATE
        jout::jout
)
