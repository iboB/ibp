#include "Frame.hpp"

#include <itlib/span.hpp>

#include <ostream>
#include <vector>
#include <cassert>

namespace ibp::profile
{

Frame::Frame(std::string_view name)
    : m_name(name)
    , m_events(256)
    , m_eventExtraDatas(256)
    , m_profileDesc(EventType::Frame, m_name.c_str())
{}

void Frame::clear() {
    assert(m_timesEntered == 0);
    if (m_events.valid()) m_events.reset();
    if (m_eventExtraDatas.valid()) m_events.reset();
    m_eventExtraStoredStrings.clear();
}

namespace {

struct ReportEntry {
    ReportEntry(const EntryDesc& d, uint64_t start) : desc(d), nsStart(start), nsEnd(0) {}
    const EntryDesc& desc;
    uint64_t nsStart;
    uint64_t nsEnd;
    std::vector<ReportEntry> children;
};

void printEntries(std::ostream& out, itlib::span<const ReportEntry> entries, int depth = 0) {
    auto t2s = [](EventType t) -> const char* {
        switch (t) {
        case EventType::Frame: return "frame";
        case EventType::Function: return "func";
        case EventType::Block: return "b";
        case EventType::BasicEvent: return "e";
        }

        return "?";
    };


    for (auto& e : entries) {
        for (int i = 0; i < depth; ++i) out << "  ";
        out << t2s(e.desc.type) << ' ';
        out << e.desc.label;
        if (e.desc.type != EventType::BasicEvent) {
            out << ": ";
            out << (e.nsEnd - e.nsStart) / 1'000'000;
            out << " ms";
        }
        out << '\n';
        printEntries(out, e.children, depth + 1);
    }
}
}

void Frame::dump(std::ostream& out) {
    if (m_events.empty()) return;

    std::vector<ReportEntry> roots;
    std::vector<ReportEntry> stack;

    for (auto& e : m_events) {
        if (e.desc) {
            if (e.desc->type == EventType::BasicEvent) {
                assert(!stack.empty());
                auto& c = stack.back().children.emplace_back(*e.desc, e.nsTimestamp);
                c.nsEnd = e.nsTimestamp;
            }
            else {
                stack.emplace_back(*e.desc, e.nsTimestamp);
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

}