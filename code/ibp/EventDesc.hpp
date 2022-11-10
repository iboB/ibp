// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

#include <cstdint>

namespace ibp
{
class IBP_API EventDesc
{
public:
    enum class Type : uint8_t
    {
        Frame, Function, Block, Event
    };

    EventDesc(Type t, const char* l);
    Type type;
    const char* label;
    uint64_t hash;
};
}
