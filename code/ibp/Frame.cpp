#include "Frame.hpp"

#include <ostream>
#include <algorithm>

namespace ibp
{

void Frame::dump(std::ostream& out)
{
    std::sort(m_blocks.begin(), m_blocks.end(), [](const Block& a, const Block& b) {
        if (a.start < b.start) return true;
        if (a.start > b.start) return false;
        return a.end < b.end;
    });
    for (auto& b : m_blocks)
    {
        out << b.desc.label << ": ";
        out << std::chrono::duration_cast<std::chrono::milliseconds>(b.end.time_since_epoch() - b.start.time_since_epoch()).count();
        out << " ms\n";
    }
}

}