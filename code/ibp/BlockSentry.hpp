// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "Instance.hpp"

namespace ibp
{

struct BlockSentry
{
public:
    BlockSentry(BlockDesc& desc)
    {
        Instance::beginBlock(desc);
    }
    ~BlockSentry()
    {
        Instance::endTopBlock();
    }
};

}
