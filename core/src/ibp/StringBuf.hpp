// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <itlib/pod_vector.hpp>
#include <itlib/span.hpp>
#include <cstdint>
#include <string_view>

namespace ibp {
class StringBuf {
public:
    struct String {
        uint32_t begin;
        uint32_t length;
    };

    String add(std::string_view str) {
        String ret;
        ret.begin = uint32_t(m_buf.size());
        ret.length = uint32_t(str.length());
        m_buf.insert(m_buf.end(), str.begin(), str.end());
        m_buf.push_back(0);
        return ret;
    }

    std::string_view get(String str) const {
        return {m_buf.data() + str.begin, str.length};
    }

    void clear() { m_buf.clear(); }

    itlib::span<const char> buf() const { return m_buf; }

    void cat(const StringBuf& other) {
        m_buf.insert(m_buf.end(), other.m_buf.begin(), other.m_buf.end());
    }
private:
    itlib::pod_vector<char> m_buf;
};
}
