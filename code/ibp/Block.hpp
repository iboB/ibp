// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "BlockDesc.hpp"
#include <chrono>

namespace ibp
{

struct Block
{
    BlockDesc desc;
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
};

}
