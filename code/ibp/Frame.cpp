#include "Frame.hpp"
#include "EventDesc.hpp"

#include <itlib/span.hpp>

#include <ostream>
#include <vector>
#include <cassert>

namespace ibp
{

Frame::Frame(std::string_view name)
    : m_events(256)
    , m_eventExtraDatas(256)
    , m_name(name)
    , m_eventDesc(EventDesc::Type::Frame, m_name.c_str())
{}

namespace
{

struct ReportEntry
{
    ReportEntry(const EventDesc& d, uint64_t start) : desc(d), nsStart(start), nsEnd(0) {}
    const EventDesc& desc;
    uint64_t nsStart;
    uint64_t nsEnd;
    std::vector<ReportEntry> children;
};

void printEntries(std::ostream& out, itlib::span<const ReportEntry> entries, int depth = 0)
{
    auto t2s = [](EventDesc::Type t) -> const char* {
        switch (t)
        {
        case EventDesc::Type::Frame: return "frame";
        case EventDesc::Type::Function: return "func";
        case EventDesc::Type::Block: return "b";
        case EventDesc::Type::Event: return "e";
        }

        return "?";
    };


    for (auto& e : entries)
    {
        for (int i = 0; i < depth; ++i) out << "  ";
        out << t2s(e.desc.type) << ' ';
        out << e.desc.label << ": ";
        out << (e.nsEnd - e.nsStart) / 1'000'000;
        out << " ms\n";
        printEntries(out, e.children, depth + 1);
    }
}
}

void Frame::dump(std::ostream& out)
{
    if (m_events.empty()) return;

    std::vector<ReportEntry> roots;
    std::vector<ReportEntry> stack;

    for (auto& e : m_events)
    {
        if (e.desc)
        {
            if (e.desc->type == EventDesc::Type::Event)
            {
                assert(!stack.empty());
                auto& c = stack.back().children.emplace_back(*e.desc, e.nsTimestamp);
                c.nsEnd = e.nsTimestamp;
            }
            else
            {
                stack.emplace_back(*e.desc, e.nsTimestamp);
            }
        }
        else
        {
            assert(!stack.empty());
            stack.back().nsEnd = e.nsTimestamp;
            auto back = std::move(stack.back());
            stack.pop_back();
            if (stack.empty())
            {
                // empty stack means the roots is complete
                roots.push_back(std::move(back));
            }
            else
            {
                // otherwise add to prev
                stack.back().children.push_back(std::move(back));
            }
        }
    }

    printEntries(out, roots);
}

}