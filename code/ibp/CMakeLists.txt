# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
icm_add_lib(ibp IBP)
add_library(ibp::ibp ALIAS ibp)
target_sources(ibp PRIVATE
    profile/ChunkedBlockStorage.hpp

    profile/EntryDesc.hpp
    profile/EntryDesc.cpp
    profile/ProfileFuncs.hpp
    profile/FrameSentry.hpp
    profile/ProfileImpl.cpp

    API.h
    Profile.hpp
    Frame.hpp
    Frame.cpp
)

target_include_directories(ibp INTERFACE ..)

target_link_libraries(ibp
    PUBLIC
        splat::splat
        itlib::itlib
    PRIVATE
        ibp::xxhash
)

