# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
icm_add_lib(ibp IBP)
add_library(ibp::ibp ALIAS ibp)
target_sources(ibp PRIVATE
    API.h
    BlockDesc.hpp
    Instance.hpp
    Instance.cpp
    Profile.hpp
    Sentry.hpp
    Frame.hpp
    Frame.cpp
)

target_include_directories(ibp INTERFACE ..)

target_link_libraries(ibp
    PUBLIC
        splat
)

