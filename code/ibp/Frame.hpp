// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

#include "BlockDesc.hpp"

#include "impl/ChunkedBlockStorage.hpp"

#include <itlib/pod_vector.hpp>
#include <iosfwd>
#include <string>

namespace ibp
{
namespace impl { class ThreadProfile; }

class IBP_API Frame
{
    friend class impl::ThreadProfile;

    // raw profile data
    struct Block
    {
        const BlockDesc* desc;
        uint64_t nsStart;
        uint64_t nsEnd;
    };
    impl::ChunkedBlockStorage<Block> m_blocks; // blocks in a "linear" tree
    itlib::pod_vector<Block*> m_stack; // stack while the frame is active

    // activation logic
    // we support reentrant begin frame
    // we support beginning a frame inside of another
    Frame* m_prevFrame = nullptr; // restore this when leaving
    int m_timesEntered = 0; // how many times begin frame has been called for this frame

    // frame desc
    std::string m_name;
    BlockDesc m_blockDesc;
public:
    Frame(std::string_view name);

    void dump(std::ostream& out);
};

}
