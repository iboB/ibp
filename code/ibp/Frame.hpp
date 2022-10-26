// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

#include "Block.hpp"

#include "impl/ChunkedBlockStorage.hpp"

#include <itlib/small_vector.hpp>
#include <iosfwd>

namespace ibp
{
namespace impl { class ThreadProfile; }

class IBP_API Frame
{
    friend class impl::ThreadProfile;
    impl::ChunkedBlockStorage<Block> m_blocks;
    itlib::small_vector<Block*, 16> m_stack;
public:
    void dump(std::ostream& out);
};

}
