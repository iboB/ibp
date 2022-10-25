#include "Frame.hpp"

#include <ostream>
#include <algorithm>

namespace ibp
{

void Frame::dump(std::ostream& out)
{
    for (auto& b : m_blocks)
    {
        out << b.desc->label << ": ";
        out << (b.nsEnd - b.nsStart) / 1'000'000;
        out << " ms\n";
    }
}

}