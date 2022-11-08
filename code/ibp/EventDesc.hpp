// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

#include <string_view>

namespace ibp
{
class IBP_API EventDesc
{
public:
    EventDesc(const char* l);
    std::string_view label;
    uint64_t hash;
};
}
