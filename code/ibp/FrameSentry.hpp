// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

namespace ibp
{
class Frame;

// implemented in ProfileImpl.cpp
class IBP_API FrameSentry
{
public:
    FrameSentry(Frame& desc);
    ~FrameSentry();
};

}
