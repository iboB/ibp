# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
icm_add_lib(ibp-profile IBP)
add_library(ibp::profile ALIAS ibp-profile)
target_sources(ibp-profile PRIVATE
    profile/ChunkedBlockStorage.hpp
    profile/API.h
    profile/EntryDesc.hpp
    profile/EntryDesc.cpp
    profile/ProfileFuncs.hpp
    profile/FrameSentry.hpp
    profile/ProfileImpl.cpp
    profile/Frame.hpp
    profile/Frame.cpp

    Profile.hpp
)

target_include_directories(ibp-profile INTERFACE ..)

ibp_dep(splat)
ibp_dep(itlib)
ibp_dep(xxhash)

target_link_libraries(ibp-profile
    PUBLIC
        ibp::core
        splat::splat
        itlib::itlib
    PRIVATE
        ibp::xxhash
)

