// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

#include "Block.hpp"

#include <vector>
#include <iosfwd>

namespace ibp
{
namespace impl { class InstanceImpl; }

class IBP_API Frame
{
    friend class impl::InstanceImpl;
    std::vector<Block> m_blocks;
public:
    void dump(std::ostream& out);
};

}
