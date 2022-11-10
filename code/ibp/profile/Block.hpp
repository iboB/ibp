// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "../API.h"

namespace ibp::profile
{
class EntryDesc;

// implemented in ProfileImpl.cpp
IBP_API void beginBlock(const EntryDesc& desc);
IBP_API void endTopBlock();

class BlockSentry
{
public:
    BlockSentry(EntryDesc& desc)
    {
        profile::beginBlock(desc);
    }
    ~BlockSentry()
    {
        profile::endTopBlock();
    }

    BlockSentry(const BlockSentry&) = delete;
    BlockSentry& operator=(const BlockSentry&) = delete;
    BlockSentry(BlockSentry&&) = delete;
    BlockSentry& operator=(BlockSentry&&) = delete;
};

}
