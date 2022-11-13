// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "EventType.hpp"
#include "StringBuf.hpp"

namespace ibp {
class EventDump
{
public:
    StringBuf strings;

    struct EventDesc {
        EventType type;
        StringBuf::String label;
    };
    itlib::pod_vector<EventDesc> eventDescs;

    static constexpr uint32_t None = ~uint32_t(0);

    struct EventExtra {
        enum class ExtraType : uint64_t {
            None, Num, String
        };
        Type type;
        union {
            uint64_t num;
            StringBuf::String string;
        };
    };
    itlib::pod_vector<EventExtra> eventExtras;

    struct Event {
        uint64_t nsTimestamp; // timestamp in ns
        uint32_t idesc; // None means end-event
        uint32_t iextra;
    };
    itlib::pod_vector<Event> events;
};
}