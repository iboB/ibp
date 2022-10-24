// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

namespace ibp
{

struct BlockDesc
{
    BlockDesc() = default;
    BlockDesc(const char* l) : label(l) {}
    const char* label;
};

}
