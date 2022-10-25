// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

namespace ibp
{

struct BlockDesc;
class Frame;

class IBP_API Instance
{
public:
    static void beginFrame(Frame& frame);

    static void beginBlock(const BlockDesc& desc);
    static void endTopBlock();
};

}