// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "PrintDump.hpp"

#include <ibp/ProfileDump.hpp>

#include <iostream>
#include <iomanip>
#include <cassert>

namespace ibp {

//std::string ms(uint64_t ns) {
//    auto t = double(ns) / 1000000.;
//    char buf[32];
//    snprintf(buf, sizeof(buf), "%4.4f", t);
//    std::string ret = buf;
//    if (ret.size() < 9) {
//        ret = std::string(9 - ret.size(), ' ') + ret;
//    };
//    return ret;
//}

double ms(uint64_t ns) {
    return double(ns) / 1000000.;
}

#define INDENT "  "

void ProfileDump_print(const ProfileDump& dump, std::ostream& out) {
    int depth = 0;
    uint64_t lastTimestamp = 0;
    bool lastWasBegin = false;
    itlib::pod_vector<uint64_t> beginTimestamps;

    for (auto& e : dump.events) {
        if (e.idesc != ibp::ProfileDump::None) {
            if (lastWasBegin) {
                out << " {\n";
            }

            for (int i=0; i<depth; ++i) {
                out << INDENT;
            }

            // not an end event
            auto& edesc = dump.eventDescs[e.idesc];
            auto type = edesc.type;
            auto label = dump.strings.get(edesc.label);
            if (type != ibp::EventType::Frame) {
                out << '+' << ms(e.nsTimestamp - lastTimestamp) << ' ';
                switch (type) {
                case ibp::EventType::Function:
                    out << "f ";
                    break;
                case ibp::EventType::Block:
                    out << "b ";
                    break;
                case ibp::EventType::BasicEvent:
                    out << "e ";
                    break;
                default:
                    out << "? ";
                    break;
                }
            }

            out << "'" << label << "'";
            if (e.iextra != ibp::ProfileDump::None) {
                out << " (";
                auto& extra = dump.eventExtras[e.iextra];
                switch (extra.type) {
                case ibp::ProfileDump::EventExtra::Type::Num:
                    out << extra.num;
                    break;
                case ibp::ProfileDump::EventExtra::Type::String: {
                    auto str = dump.strings.get(extra.string);
                    out << "'" << str << "'";
                    break;
                }
                default:
                    out << "?";
                    break;
                }
                out << ")";
            }

            if (type != ibp::EventType::BasicEvent) {
                ++depth;
                lastWasBegin = true;
                beginTimestamps.push_back(e.nsTimestamp);
            }
            else {
                out << '\n';
                lastWasBegin = false;
            }
        }
        else {
            assert(depth);
            --depth;

            if (!lastWasBegin) {
                for (int i = 0; i < depth; ++i) {
                    out << INDENT;
                }
                out << INDENT "+" << ms(e.nsTimestamp - lastTimestamp) << '\n';
                for (int i = 0; i < depth; ++i) {
                    out << INDENT;
                }
                out << '}';
            }

            out << ": " << ms(e.nsTimestamp - beginTimestamps.back()) << '\n';

            assert(!beginTimestamps.empty());
            beginTimestamps.pop_back();
            lastWasBegin = false;
        }

        lastTimestamp = e.nsTimestamp;
    }
}

}

