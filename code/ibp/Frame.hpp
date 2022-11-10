// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

#include "EventDesc.hpp"

#include "impl/ChunkedBlockStorage.hpp"

#include <itlib/pod_vector.hpp>
#include <iosfwd>
#include <string>

namespace ibp
{
namespace impl { class ThreadProfile; }

class IBP_API Frame
{
public:
    Frame(std::string_view name);

    // enable only affects the frame sentry
    // disabling the frame while it's set as current has no effect
    bool enabled() const { return m_enabled; }
    void setEnabled(bool b = true) { m_enabled = b; }

    // temp
    void dump(std::ostream& out);
private:
    friend class impl::ThreadProfile;

    // raw profile data
    struct Event
    {
        const EventDesc* desc;
        uint64_t nsTimestamp;
    };
    impl::ChunkedBlockStorage<Event> m_events;

    // activation logic
    // we support reentrant begin frame
    // we support beginning a frame inside of another
    Frame* m_prevFrame = nullptr; // restore this when leaving
    int m_timesEntered = 0; // how many times begin frame has been called for this frame
    bool m_enabled = false;

    // frame desc
    std::string m_name;
    EventDesc m_eventDesc;
};

}
