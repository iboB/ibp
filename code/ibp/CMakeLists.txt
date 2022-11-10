# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
icm_add_lib(ibp IBP)
add_library(ibp::ibp ALIAS ibp)
target_sources(ibp PRIVATE
    impl/ChunkedBlockStorage.hpp
    API.h
    EventDesc.hpp
    EventDesc.cpp
    BlockSentry.hpp
    Event.hpp
    Profile.hpp
    Frame.hpp
    Frame.cpp
    FrameSentry.hpp
    ProfileImpl.cpp
)

target_include_directories(ibp INTERFACE ..)

target_link_libraries(ibp
    PUBLIC
        splat::splat
        itlib::itlib
    PRIVATE
        ibp::xxhash
)

