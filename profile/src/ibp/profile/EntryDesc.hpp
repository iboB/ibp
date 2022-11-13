// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

#include <ibp/EventType.hpp>

#include <cstdint>
#include <optional>

namespace ibp::profile {
class IBP_API EntryDesc {
public:
    EntryDesc(EventType t, const char* l);
    EventType type;
    const char* label;

    uint64_t hash() const;
private:
    mutable std::optional<uint64_t> m_hash;
};
}
