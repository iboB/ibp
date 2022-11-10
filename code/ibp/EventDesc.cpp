// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "EventDesc.hpp"

#define XXH_STATIC_LINKING_ONLY
#define XXH_IMPLEMENTATION
#include <xxhash.h>

namespace ibp
{

EventDesc::EventDesc(Type t, const char* l)
    : label(l)
    , type(t)
{
    auto len = strlen(label);
    XXH64_state_t hstate;
    XXH64_reset(&hstate, 0);
    XXH64_update(&hstate, label, len);
    XXH64_update(&hstate, &type, 1);
    hash = XXH64_digest(&hstate);
}

}