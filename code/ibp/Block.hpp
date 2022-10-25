// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "BlockDesc.hpp"

#include <cstdint>

namespace ibp
{

struct Block
{
    const BlockDesc* desc;
    uint64_t nsStart;
    uint64_t nsEnd;
};

}
