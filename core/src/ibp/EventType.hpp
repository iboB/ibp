// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <cstdint>

namespace ibp {
enum class EventType : uint8_t {
    // endable events
    Frame, // a frame, frames within a dump can be disjoint
    Function, // a function (the name may require special parsing)
    Block, // a user block (name is interpreted verbatim)

    // instant events
    BasicEvent,
};
}
