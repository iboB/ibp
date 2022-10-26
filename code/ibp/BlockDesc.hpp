// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

namespace ibp
{

class BlockDesc
{
public:
    BlockDesc() = default;
    BlockDesc(const char* l) : label(l) {}
    const char* label;
};

}
