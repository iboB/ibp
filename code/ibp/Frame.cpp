#include "Frame.hpp"

#include <ostream>
#include <algorithm>

namespace ibp
{

void Frame::dump(std::ostream& out)
{
    std::sort(m_blocks.begin(), m_blocks.end(), [](const Block& a, const Block& b) {
        if (a.nsStart < b.nsStart) return true;
        if (a.nsStart > b.nsStart) return false;
        return a.nsEnd < b.nsEnd;
    });
    for (auto& b : m_blocks)
    {
        out << b.desc->label << ": ";
        out << (b.nsEnd - b.nsStart) / 1'000'000;
        out << " ms\n";
    }
}

}