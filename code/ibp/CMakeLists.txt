# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
icm_add_lib(ibp IBP)
add_library(ibp::ibp ALIAS ibp)
target_sources(ibp PRIVATE
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

target_include_directories(ibp INTERFACE ..)

ibp_dep(splat)
ibp_dep(itlib)
ibp_dep(xxhash)

target_link_libraries(ibp
    PUBLIC
        splat::splat
        itlib::itlib
    PRIVATE
        ibp::xxhash
)

