#include "BlockSentry.hpp"
#include "FrameSentry.hpp"
#include "Frame.hpp"

#include <chrono>
#include <cassert>

namespace ibp
{
namespace impl
{

using clock = std::chrono::steady_clock;

class ThreadProfile
{
public:
    Frame* frame;

    void beginFrame(Frame& f)
    {
        frame = &f;
        if (!f.m_blocks.valid())
        {
            f.m_blocks.init();
            f.m_stack.reserve(16);
        }
    }

    void endTopFrame()
    {
        frame = nullptr;
    }

    void beginBlock(const BlockDesc& desc)
    {
        if (!frame) return;
        auto& b = frame->m_blocks.emplace_back();
        b.desc = &desc;
        frame->m_stack.push_back(&b);
        b.nsStart = clock::now().time_since_epoch().count();
    }

    void endTopBlock()
    {
        if (!frame) return;
        auto end = clock::now();
        frame->m_stack.back()->nsEnd = end.time_since_epoch().count();
        frame->m_stack.pop_back();
    }
};

}

static thread_local impl::ThreadProfile thread;

namespace profile
{
void beginBlock(const BlockDesc& desc) { thread.beginBlock(desc); }
void endTopBlock() { thread.endTopBlock(); }
}

FrameSentry::FrameSentry(Frame& f) { thread.beginFrame(f); }
FrameSentry::~FrameSentry() { thread.endTopFrame(); }

}
