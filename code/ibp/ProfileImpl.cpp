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

        // init frame in case it's the first time it has been entered
        if (!f.m_events.valid())
        {
            f.m_events.init();
        }

        frame = &f;
        beginBlock(frame->m_eventDesc);
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

        auto prev = frame->m_prevFrame;

        // clear
        frame->m_timesEntered = 0;
        frame->m_prevFrame = nullptr;

        // restore previous (if any)
        frame = prev;
    }

    void beginBlock(const EventDesc& desc)
    {
        if (!frame) return; // safe - no frame
        auto& b = frame->m_events.emplace_back();
        b.desc = &desc;
        b.nsTimestamp = clock::now().time_since_epoch().count(); // start time at the last possible moment
    }

    void endTopBlock()
    {
        if (!frame) return; // safe - no frame
        // end time at the first possible moment...
        auto end = clock::now();
        // ... thus potential cache misses in the code below won't affect it
        auto& e = frame->m_events.emplace_back();
        e.desc = nullptr;
        e.nsTimestamp = end.time_since_epoch().count();
    }
};

}

static thread_local impl::ThreadProfile thread;

namespace profile
{
void beginBlock(const EventDesc& desc) { thread.beginBlock(desc); }
void endTopBlock() { thread.endTopBlock(); }
}

FrameSentry::FrameSentry(Frame& f)
    : m_frameStarted(f.enabled())
{
    if (m_frameStarted) thread.beginFrame(f);
}

FrameSentry::~FrameSentry()
{
    if (!m_frameStarted) return;
    thread.endTopFrame();
}

}
