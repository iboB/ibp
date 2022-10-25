// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "Instance.hpp"
#include "Frame.hpp"

#include <vector>
#include <chrono>

namespace ibp
{
namespace impl
{

using clock = std::chrono::steady_clock;

class InstanceImpl
{
public:
    Frame* m_curFrame = nullptr;
    std::vector<Block> m_stack;

    void beginFrame(Frame& frame)
    {
        m_curFrame = &frame;
    }

    void beginBlock(BlockDesc& desc)
    {
        if (!m_curFrame) return;
        auto& b = m_stack.emplace_back();
        b.desc = desc;
        b.nsStart = clock::now().time_since_epoch().count();
    }

    void endTopBlock()
    {
        if (!m_curFrame) return;
        auto& b = m_stack.back();
        b.nsEnd = clock::now().time_since_epoch().count();;
        m_curFrame->m_blocks.push_back(std::move(b));
        m_stack.pop_back();
    }

};

InstanceImpl i;
}

void Instance::beginFrame(Frame& frame) { impl::i.beginFrame(frame); }
void Instance::beginBlock(BlockDesc& desc) { impl::i.beginBlock(desc); }
void Instance::endTopBlock() { impl::i.endTopBlock(); }

}
