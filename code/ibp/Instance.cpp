// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "Instance.hpp"
#include "Frame.hpp"

#include <vector>

namespace ibp
{
namespace impl
{
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
        b.start = std::chrono::steady_clock::now();
    }

    void endTopBlock()
    {
        if (!m_curFrame) return;
        auto& b = m_stack.back();
        b.end = std::chrono::steady_clock::now();
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
