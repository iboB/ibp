// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

#include "Block.hpp"

#include "impl/ChunkedBlockStorage.hpp"

#include <iosfwd>

namespace ibp
{
namespace impl { class InstanceImpl; }

class IBP_API Frame
{
    friend class impl::InstanceImpl;
    impl::ChunkedBlockStorage<Block> m_blocks;
public:
    void dump(std::ostream& out);
};

}
