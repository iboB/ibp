#include "Frame.hpp"
#include "BlockDesc.hpp"

#include <ostream>
#include <vector>

namespace ibp
{

Frame::Frame(std::string_view name)
    : m_name(name)
    , m_blockDesc(m_name.c_str())
{}

void Frame::dump(std::ostream& out)
{
    if (m_blocks.empty()) return;

    std::vector<uint64_t> frameEndsStack;

    for (auto& b : m_blocks)
    {
        while (!frameEndsStack.empty() && frameEndsStack.back() <= b.nsStart) frameEndsStack.pop_back();

        for (size_t i = 0; i < frameEndsStack.size(); ++i) out << "  ";

        out << b.desc->label << ": ";
        out << (b.nsEnd - b.nsStart) / 1'000'000;
        out << " ms\n";

        frameEndsStack.push_back(b.nsEnd);
    }
}

}