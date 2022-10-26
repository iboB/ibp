// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <cstdint>

namespace ibp
{
class BlockDesc;
struct Block
{
    const BlockDesc* desc;
    uint64_t nsStart;
    uint64_t nsEnd;
};

}
