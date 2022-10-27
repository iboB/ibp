#include "Frame.hpp"
#include "BlockDesc.hpp"

#include <ostream>
#include <algorithm>

namespace ibp
{

Frame::Frame(std::string_view name)
    : m_name(name)
    , m_blockDesc(m_name.c_str())
{}

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