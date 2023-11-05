#include "ProfileFuncs.hpp"
#include "FrameSentry.hpp"
#include "Frame.hpp"

#include <chrono>
#include <cassert>

namespace ibp::profile {

using clock = std::chrono::steady_clock;

class ThreadProfile {
public:
    Frame* frame;

    void beginFrame(Frame& f) {
        if (frame == &f) {
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
        if (!f.m_events.valid()) {
            f.m_events.init();
        }

        frame = &f;
        newEvent(frame->m_profileDesc);
    }

    void endTopFrame() {
        assert(frame);

        if (frame->m_timesEntered > 1) {
            --frame->m_timesEntered;
            return; // nothing more to do
        }

        endEvent();

        assert(frame->m_timesEntered == 1);

        auto prev = frame->m_prevFrame;

        // clear
        frame->m_timesEntered = 0;
        frame->m_prevFrame = nullptr;

        // restore previous (if any)
        frame = prev;
    }

    template <typename ExtraDataFunc>
    void newEvent(const EntryDesc& desc, ExtraDataFunc extraFunc) {
        if (!frame) return; // safe - no frame
        auto& e = frame->m_events.emplace_back();
        e.desc = &desc;
        e.extra = extraFunc();
        e.nsTimestamp = clock::now().time_since_epoch().count(); // start time at the last possible moment
    }

    void newEvent(const EntryDesc& desc) {
        newEvent(desc, []() -> Frame::Event::ExtraData* { return nullptr; });
    }

    void newEventIntExtra(const EntryDesc& desc, int64_t extra) {
        newEvent(desc, [&]() { return &newExtraDataNum(extra); });
    }

    void newEventLiteralExtra(const EntryDesc& desc, std::string_view extra) {
        newEvent(desc, [&]() { return &newExtraDataExternalString(extra); });
    }

    void newEventStringExtra(const EntryDesc& desc, std::string_view extra) {
        newEvent(desc, [&]() { return &newExtraDataStoredString(std::string(extra)); });
    }

    void endEvent() {
        if (!frame) return; // safe - no frame
        // end time at the first possible moment...
        auto end = clock::now();
        // ... thus potential cache misses in the code below won't affect it
        auto& e = frame->m_events.emplace_back();
        e.desc = nullptr;
        e.nsTimestamp = end.time_since_epoch().count();
        e.extra = nullptr;
    }

    Frame::Event::ExtraData& newExtraData() {
        if (!frame->m_eventExtraDatas.valid()) {
            frame->m_eventExtraDatas.init();
        }
        return frame->m_eventExtraDatas.emplace_back();
    }

    Frame::Event::ExtraData& newExtraDataNum(int64_t num) {
        // shouldn't be possible to call with no frame
        assert(frame);
        auto& ret = newExtraData();
        ret.num = num;
        ret.string = nullptr;
        return ret;
    }

    Frame::Event::ExtraData& newExtraDataExternalString(std::string_view str) {
        // shouldn't be possible to call with no frame
        assert(frame);
        auto& ret = newExtraData();
        ret.num = int64_t(str.length());;
        ret.string = str.data();
        return ret;
    }

    Frame::Event::ExtraData& newExtraDataStoredString(std::string str) {
        // shouldn't be possible to call with no frame
        assert(frame);
        auto& added = frame->m_eventExtraStoredStrings.emplace_back(std::move(str));
        auto& ret = newExtraData();
        ret.num = int64_t(added.length());
        ret.string = added.c_str();
        return ret;
    }
};

static thread_local ThreadProfile thread;

void newEvent(const EntryDesc& desc) { thread.newEvent(desc); }
void newEvent(const EntryDesc& desc, Int extra) { thread.newEventIntExtra(desc, extra.i); }
void newEvent(const EntryDesc& desc, Literal extra) { thread.newEventLiteralExtra(desc, extra.str); }
void newEvent(const EntryDesc& desc, String extra) { thread.newEventStringExtra(desc, extra.str); }
void endEvent() { thread.endEvent(); }

FrameSentry::FrameSentry(Frame& f)
    : m_frameStarted(f.enabled())
{
    if (m_frameStarted) thread.beginFrame(f);
}

FrameSentry::~FrameSentry() {
    if (!m_frameStarted) return;
    thread.endTopFrame();
}

}
