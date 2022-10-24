// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "Instance.hpp"

namespace ibp
{

struct Sentry
{
public:
    Sentry(BlockDesc& desc)
    {
        Instance::beginBlock(desc);
    }
    ~Sentry()
    {
        Instance::endTopBlock();
    }
};

}
