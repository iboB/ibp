// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

#include "EntryDesc.hpp"
#include "ChunkedBlockStorage.hpp"

#include <iosfwd>
#include <string>
#include <deque>

namespace ibp::profile {
class ThreadProfile;

class IBP_API Frame {
public:
    Frame(std::string_view name);

    // enable only affects the frame sentry
    // disabling the frame while it's set as current has no effect
    bool enabled() const { return m_enabled; }
    void setEnabled(bool b = true) { m_enabled = b; }

    // clear all collected data
    // call this if you reuse the frame
    void clear();

    // temp
    void dump(std::ostream& out);
private:
    std::string m_name;

    friend class ThreadProfile;

    // raw profile data
    struct Event {
        const EntryDesc* desc; // will be null for block end
        uint64_t nsTimestamp;

        struct ExtraData {
            int64_t num; // if string is not null, this is used as length
            const char* string;
        };
        ExtraData* extra; // null if no extra data
    };
    ChunkedBlockStorage<Event> m_events;
    ChunkedBlockStorage<Event::ExtraData> m_eventExtraDatas;

    // this can be redesigned to a free-list type of struct
    std::deque<std::string> m_eventExtraStoredStrings;

    // activation logic
    // we support reentrant begin frame
    // we support beginning a frame inside of another
    Frame* m_prevFrame = nullptr; // restore this when leaving
    int m_timesEntered = 0; // how many times begin frame has been called for this frame
    bool m_enabled = false;

    // frame desc
    EntryDesc m_profileDesc;
};

}
