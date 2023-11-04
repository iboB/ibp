# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
icm_add_lib(ibp-print-dump IBP_PRINT_DUMP)
add_library(ibp::print-dump ALIAS ibp-print-dump)
target_sources(ibp-print-dump PRIVATE
    PrintDump.hpp
    PrintDump.cpp
)
target_include_directories(ibp-print-dump INTERFACE ..)

ibp_dep(splat)

target_link_libraries(ibp-print-dump
    PUBLIC
        ibp::core
        splat::splat
)
