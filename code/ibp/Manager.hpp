// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

namespace ibp
{

struct BlockDesc;

class IBP_API Manager
{
public:
    static Manager& i();

    void beginBlock(BlockDesc& desc);
    void endTopBlock();
};

}