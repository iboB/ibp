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
    std::vector<Block*> m_stack;

    void beginFrame(Frame& frame)
    {
        m_curFrame = &frame;
    }

    void beginBlock(const BlockDesc& desc)
    {
        if (!m_curFrame) return;
        auto& b = m_curFrame->m_blocks.emplace_back();
        b.desc = &desc;
        m_stack.push_back(&b);
        b.nsStart = clock::now().time_since_epoch().count();
    }

    void endTopBlock()
    {
        if (!m_curFrame) return;
        auto end = clock::now();
        m_stack.back()->nsEnd = end.time_since_epoch().count();
        m_stack.pop_back();
    }

};

InstanceImpl i;
}

void Instance::beginFrame(Frame& frame) { impl::i.beginFrame(frame); }
void Instance::beginBlock(const BlockDesc& desc) { impl::i.beginBlock(desc); }
void Instance::endTopBlock() { impl::i.endTopBlock(); }

}
