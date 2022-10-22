# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(profile)

# cpm
include(./get_cpm.cmake)

#######################################
# cmake lib
CPMAddPackage(gh:iboB/icm@1.4.1)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${icm_SOURCE_DIR}
)

include(icm_dev_mode)
include(icm_add_lib)

#######################################
# options
option(PROFILE_STATIC "profile: build as static lib" OFF)
option(PROFILE_BUILD_TESTS "profile: build tests" ${ICM_DEV_MODE})
option(PROFILE_BUILD_EXAMPLES "profile: build examples" ${ICM_DEV_MODE})

#######################################
# packages
CPMAddPackage(gh:iboB/splat@1.2.1)
CPMAddPackage(gh:iboB/itlib@1.6.3)

#######################################
# subdirs

add_subdirectory(code/profile)

if(ICM_DEV_MODE)
    add_subdirectory(scratch)
endif()

if(PROFILE_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(PROFILE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
