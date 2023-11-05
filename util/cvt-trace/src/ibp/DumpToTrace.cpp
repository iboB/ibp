// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "DumpToTrace.hpp"

#include <ibp/ProfileDump.hpp>

#include <jout/jout.hpp>

namespace ibp {

void ProfileDump_toTraceJson(const ProfileDump& dump, std::ostream& out) {
    jout::Document json(out, true);
    auto root = json.obj();

    root.val("displayTimeUnit", "ns");

    // every new frame in the dump is represented with a different thead id
    int curtid = 0;

    auto jevents = root.ar("traceEvents");
    for (auto& e : dump.events) {
        auto je = jevents.obj();
        je.val("ts", e.nsTimestamp / 1000); // microseconds
        if (e.idesc != ibp::ProfileDump::None) {
            auto& desc = dump.eventDescs[e.idesc];
            auto type = desc.type;
            auto label = dump.strings.get(desc.label);
            je.val("name", label);
            switch (type) {
            case EventType::Frame:
                ++curtid;
                [[fallthrough]];
            case EventType::Function: [[fallthrough]];
            case EventType::Block:
                je.val("ph", "B");
                break;
            case EventType::BasicEvent:
                je.val("ph", "i");
                break;
            }
            if (e.iextra != ibp::ProfileDump::None) {
                auto args = je.obj("args");
                auto& extra = dump.eventExtras[e.iextra];
                if (extra.type == ProfileDump::EventExtra::Type::Num) {
                    je.val("n", extra.num);
                }
                else if(extra.type == ProfileDump::EventExtra::Type::String) {
                    je.val("s", dump.strings.get(extra.string));
                }
            }
        }
        else {
            je.val("ph", "E");
        }
        je.val("tid", curtid);
        je.val("pid", 0);
    }
}

}

