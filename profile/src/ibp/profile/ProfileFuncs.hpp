// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

namespace ibp::profile
{
class EntryDesc;

// implemented in ProfileImpl.cpp
IBP_API void newEvent(const EntryDesc& desc);
IBP_API void endEvent();

class BlockSentry
{
public:
    BlockSentry(EntryDesc& desc)
    {
        profile::newEvent(desc);
    }
    ~BlockSentry()
    {
        profile::endEvent();
    }

    BlockSentry(const BlockSentry&) = delete;
    BlockSentry& operator=(const BlockSentry&) = delete;
    BlockSentry(BlockSentry&&) = delete;
    BlockSentry& operator=(BlockSentry&&) = delete;
};

}