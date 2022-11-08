// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

namespace ibp
{
class EventDesc;

namespace profile
{
// implemented in ProfileImpl.cpp
IBP_API void beginBlock(const EventDesc& desc);
IBP_API void endTopBlock();
}

class BlockSentry
{
public:
    BlockSentry(EventDesc& desc)
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
