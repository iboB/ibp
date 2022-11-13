#include "Frame.hpp"

#include <ibp/ProfileDump.hpp>

#include <itlib/span.hpp>

#include <cassert>
#include <unordered_map>

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

ProfileDump Frame::dump() const {
    ProfileDump ret;

    std::unordered_map<uint64_t, size_t> descHashToIndex;

    for (auto& fe : m_events) {
        auto& de = ret.events.emplace_back();
        de.nsTimestamp = fe.nsTimestamp;
        if (!fe.desc) { // end
            de.idesc = ProfileDump::None;
            de.iextra = ProfileDump::None;
            continue;
        }

        de.idesc = [&]() {
            auto& fdesc = *fe.desc;
            auto dhash = fdesc.hash();
            auto f = descHashToIndex.find(dhash);
            if (f != descHashToIndex.end()) return uint32_t(f->second);

            auto idesc = ret.eventDescs.size();
            auto& ddesc = ret.eventDescs.emplace_back();
            ddesc.type = fdesc.type;
            ddesc.label = ret.strings.addString(fdesc.label);
            return uint32_t(idesc);
        }();

        if (!fe.extra) {
            de.iextra = ProfileDump::None;
            continue;
        }

        de.iextra = uint32_t(ret.eventExtras.size());
        auto& dextra = ret.eventExtras.emplace_back();
        if (fe.extra->string) {
            dextra.type = ProfileDump::EventExtra::Type::String;
            dextra.string = ret.strings.addString({fe.extra->string, size_t(fe.extra->num)});
        }
        else {
            dextra.type = ProfileDump::EventExtra::Type::Num;
            dextra.num = fe.extra->num;
        }
    }

    return ret;
}

}