// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "EventType.hpp"
#include "StringBuf.hpp"

namespace ibp {
class ProfileDump
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
        enum class Type : uint64_t {
            Num, String
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
        uint32_t iextra; // None means no extra data
    };
    itlib::pod_vector<Event> events;

    void cat(const ProfileDump& other) {
        auto oldBufSize = uint32_t(strings.buf().size());
        strings.cat(other.strings);
        auto oldDescSize = uint32_t(eventDescs.size());
        auto ed0 = eventDescs.insert(eventDescs.end(), other.eventDescs.begin(), other.eventDescs.end());
        for (auto it = ed0; it != eventDescs.end(); ++it) {
            it->label.begin += oldBufSize;
        }
        auto oldExtraSize = uint32_t(eventExtras.size());
        auto ee0 = eventExtras.insert(eventExtras.end(), other.eventExtras.begin(), other.eventExtras.end());
        for (auto it = ee0; it != eventExtras.end(); ++it) {
            if (it->type == EventExtra::Type::String) {
                it->string.begin += oldBufSize;
            }
        }
        auto e0 = events.insert(events.end(), other.events.begin(), other.events.end());
        for (auto it = e0; it != events.end(); ++it) {
            if (it->idesc != None) {
                it->idesc += oldDescSize;
            }
            if (it->iextra != None) {
                it->iextra += oldExtraSize;
            }
        }
    }
};
}