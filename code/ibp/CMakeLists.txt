# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
icm_add_lib(ibp IBP)
add_library(ibp::ibp ALIAS ibp)
target_sources(ibp PRIVATE
    API.h
    BlockDesc.hpp
    Manager.hpp
    Manager.cpp
    Profile.hpp
    Sentry.hpp
)

target_include_directories(ibp INTERFACE ..)

target_link_libraries(ibp
    PUBLIC
        splat
)

