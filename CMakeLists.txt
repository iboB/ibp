# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(ibp)

#######################################
# cpm
include(./get_cpm.cmake)
macro(ibp_dep packageName)
    include(${PROJECT_SOURCE_DIR}/cpm/${packageName}.cmake)
endmacro()

#######################################
# cmake lib
CPMAddPackage(gh:iboB/icm@1.4.6)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${icm_SOURCE_DIR}
)

include(icm_dev_mode)
include(icm_add_lib)

#######################################
# options
option(IBP_STATIC "ibp: build as static lib" OFF)
option(IBP_BUILD_TESTS "ibp: build tests" ${ICM_DEV_MODE})
option(IBP_BUILD_EXAMPLES "ibp: build examples" ${ICM_DEV_MODE})

if(IBP_BUILD_TESTS)
    enable_testing()
endif()

#######################################
# subdirs

add_subdirectory(core)
add_subdirectory(profile)
add_subdirectory(util)

if(ICM_DEV_MODE)
    add_subdirectory(scratch)
endif()

#~ if(IBP_BUILD_EXAMPLES)
#~     add_subdirectory(example)
#~ endif()
