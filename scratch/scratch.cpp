// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <ibp/Profile.hpp>
#include <ibp/profile/Frame.hpp>
#include <ibp/profile/FrameSentry.hpp>

#include <ibp/ProfileDump.hpp>
#include <ibp/DumpToTrace.hpp>

#include <iostream>
#include <fstream>
#include <thread>
#include <vector>

void bar() {
    IBPROFILE_FUNC();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    IBPROFILE_BLOCK_BEGIN("block");
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    IBPROFILE_BLOCK_END();
    IBPROFILE_EVENT("endbar");
}

void foo(int i) {
    IBPROFILE_FUNC();
    std::this_thread::sleep_for(std::chrono::milliseconds(i * 10));
    if (i == 2) {
        bar();
    }
    if (i == 1) {
        IBPROFILE_EVENT("foo 1");
    }
}

struct ReportEntry {
    ReportEntry(ibp::EventType t, std::string_view l, uint64_t start) : type(t), label(l), nsStart(start), nsEnd(0) {}
    ibp::EventType type;
    std::string label;
    uint64_t nsStart;
    uint64_t nsEnd;
    std::vector<ReportEntry> children;
};

void printEntries(std::ostream& out, itlib::span<const ReportEntry> entries, int depth = 0) {
    auto t2s = [](ibp::EventType t) -> const char* {
        switch (t) {
        case ibp::EventType::Frame: return "frame";
        case ibp::EventType::Function: return "func";
        case ibp::EventType::Block: return "b";
        case ibp::EventType::BasicEvent: return "e";
        }

        return "?";
    };


    for (auto& e : entries) {
        for (int i = 0; i < depth; ++i) out << "  ";
        out << t2s(e.type) << ' ';
        out << e.label;
        if (e.type != ibp::EventType::BasicEvent) {
            out << ": ";
            out << (e.nsEnd - e.nsStart) / 1'000'000;
            out << " ms";
        }
        out << '\n';
        printEntries(out, e.children, depth + 1);
    }
}

void log(std::ostream& out, const ibp::ProfileDump& dump) {
    if (dump.events.empty()) return;

    std::vector<ReportEntry> roots;
    std::vector<ReportEntry> stack;

    for (auto& e : dump.events) {
        if (e.idesc != ibp::ProfileDump::None) {
            auto& edesc = dump.eventDescs[e.idesc];
            auto type = edesc.type;
            auto label = dump.strings.getString(edesc.label);
            if (edesc.type == ibp::EventType::BasicEvent) {
                assert(!stack.empty());
                auto& c = stack.back().children.emplace_back(type, label, e.nsTimestamp);
                c.nsEnd = e.nsTimestamp;
            }
            else {
                stack.emplace_back(type, label, e.nsTimestamp);
            }
        }
        else {
            assert(!stack.empty());
            stack.back().nsEnd = e.nsTimestamp;
            auto back = std::move(stack.back());
            stack.pop_back();
            if (stack.empty()) {
                // empty stack means the roots is complete
                roots.push_back(std::move(back));
            }
            else {
                // otherwise add to prev
                stack.back().children.push_back(std::move(back));
            }
        }
    }

    printEntries(out, roots);
}

int main() {
    ibp::profile::Frame frame("main");
    frame.setEnabled();
    {
        ibp::profile::FrameSentry f(frame);
        for (int i = 0; i < 3; ++i) {
            ibp::profile::FrameSentry f(frame);
            foo(i + 1);
        }
    }
    auto d = frame.dump();
    log(std::cout, d);

    std::ofstream fout("foo.json");
    ibp::ProfileDump_toTraceJson(d, fout);

    return 0;
}
