// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "EntryDesc.hpp"

#define XXH_STATIC_LINKING_ONLY
#define XXH_IMPLEMENTATION
#include <xxhash.h>

namespace ibp::profile
{

EntryDesc::EntryDesc(EventType t, const char* l)
    : label(l)
    , type(t)
{}

uint64_t EntryDesc::hash() const {
    if (m_hash) return *m_hash;
    XXH64_state_t hstate;
    XXH64_reset(&hstate, 0);
    auto len = strlen(label);
    XXH64_update(&hstate, label, len);
    XXH64_update(&hstate, &type, sizeof(EventType));
    return m_hash.emplace(XXH64_digest(&hstate));
}

}