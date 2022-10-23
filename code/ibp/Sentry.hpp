// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "Manager.hpp"

namespace ibp
{

struct Sentry
{
public:
    Sentry(BlockDesc& desc)
    {
        Manager::i().beginBlock(desc);
    }
    ~Sentry()
    {
        Manager::i().endTopBlock();
    }
};

}
