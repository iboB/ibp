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
    , m_name(name)
    , m_eventDesc(m_name.c_str())
{}

namespace
{
struct ReportBlock {
    ReportBlock(const EventDesc& d, uint64_t start) : desc(d), nsStart(start), nsEnd(0) {}
    const EventDesc& desc;
    uint64_t nsStart;
    uint64_t nsEnd;
    std::vector<ReportBlock> children;
};

void printBlocks(std::ostream& out, itlib::span<const ReportBlock> blocks, int depth = 0)
{
    for (auto& b : blocks)
    {
        for (int i = 0; i < depth; ++i) out << "  ";
        out << b.desc.label << ": ";
        out << (b.nsEnd - b.nsStart) / 1'000'000;
        out << " ms\n";
        printBlocks(out, b.children, depth + 1);
    }
}
}

void Frame::dump(std::ostream& out)
{
    if (m_events.empty()) return;

    std::vector<ReportBlock> roots;
    std::vector<ReportBlock> stack;

    for (auto& e : m_events)
    {
        if (e.desc)
        {
            stack.emplace_back(*e.desc, e.nsTimestamp);
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

    printBlocks(out, roots);
}

}