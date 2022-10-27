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
        if (frame == &f)
        {
            // reentering frame
            assert(frame->m_timesEntered > 0);
            ++frame->m_timesEntered;
            return;
        }

        // these assertions might also be triggered
        // if there's a bug and the same frame is concurrently entered from two different threads
        // in such a case there would be also a race here
        // such functionality is of course not supported
        assert(f.m_timesEntered == 0);
        f.m_timesEntered = 1;
        assert(!f.m_prevFrame);
        f.m_prevFrame = frame;
        assert(f.m_stack.empty());

        // init frame in case it's the first time it has been entered
        if (!f.m_blocks.valid())
        {
            f.m_blocks.init();
            f.m_stack.reserve(16);
        }

        frame = &f;
        beginBlock(frame->m_blockDesc);
    }

    void endTopFrame()
    {
        assert(frame);

        if (frame->m_timesEntered > 1)
        {
            --frame->m_timesEntered;
            return; // nothing more to do
        }

        endTopBlock();

        assert(frame->m_timesEntered == 1);
        assert(frame->m_stack.empty()); // cannot leave with a non-empty stack

        auto prev = frame->m_prevFrame;

        // clear
        frame->m_timesEntered = 0;
        frame->m_prevFrame = nullptr;

        // restore previous (if any)
        frame = prev;
    }

    void beginBlock(const BlockDesc& desc)
    {
        if (!frame) return; // safe - no frame
        auto& b = frame->m_blocks.emplace_back();
        b.desc = &desc;
        frame->m_stack.push_back(&b);
        b.nsStart = clock::now().time_since_epoch().count(); // start time at the last possible moment
    }

    void endTopBlock()
    {
        if (!frame) return; // safe - no frame
        auto end = clock::now(); // end time at the first possible moment
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

FrameSentry::FrameSentry(Frame& f)
    : m_frameStarted(f.active())
{
    if (m_frameStarted) thread.beginFrame(f);
}

FrameSentry::~FrameSentry()
{
    if (!m_frameStarted) return;
    thread.endTopFrame();
}

}
