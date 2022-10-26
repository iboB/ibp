// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

namespace ibp
{
class BlockDesc;

namespace profile
{
// implemented in ProfileImpl.cpp
IBP_API void beginBlock(const BlockDesc& desc);
IBP_API void endTopBlock();
}

class BlockSentry
{
public:
    BlockSentry(BlockDesc& desc)
    {
        profile::beginBlock(desc);
    }
    ~BlockSentry()
    {
        profile::endTopBlock();
    }
};

}
