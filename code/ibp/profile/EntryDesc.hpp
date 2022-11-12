// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "../API.h"

#include <cstdint>
#include <optional>

namespace ibp::profile
{
class IBP_API EntryDesc
{
public:
    enum class Type : uint8_t
    {
        Frame, Function, Block, Event
    };

    EntryDesc(Type t, const char* l);
    Type type;
    const char* label;

    uint64_t hash() const;
private:
    mutable std::optional<uint64_t> m_hash;
};
}
