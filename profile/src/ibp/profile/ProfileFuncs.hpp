// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

#include <cstdint>
#include <string_view>

namespace ibp::profile {
class EntryDesc;

struct Int {
    int64_t i;
    explicit Int(int64_t i) noexcept : i(i) {}
};

struct Literal {
    std::string_view str;
    template <int N>
    explicit constexpr Literal(const char(&str)[N]) noexcept : str(str, N-1) {}
};

struct String {
    std::string_view str;
    explicit String(std::string_view str) noexcept : str(str) {}
};

// implemented in ProfileImpl.cpp
IBP_API void newEvent(const EntryDesc& desc);
IBP_API void newEvent(const EntryDesc& desc, Int i);
IBP_API void newEvent(const EntryDesc& desc, Literal str);
IBP_API void newEvent(const EntryDesc& desc, String str);

IBP_API void endEvent();

class BlockSentry {
public:
    BlockSentry(EntryDesc& desc) { profile::newEvent(desc); }
    BlockSentry(EntryDesc& desc, Int i) { profile::newEvent(desc, i); }
    BlockSentry(EntryDesc& desc, Literal str) { profile::newEvent(desc, str); }
    BlockSentry(EntryDesc& desc, String str) { profile::newEvent(desc, str); }

    ~BlockSentry() {
        profile::endEvent();
    }

    BlockSentry(const BlockSentry&) = delete;
    BlockSentry& operator=(const BlockSentry&) = delete;
    BlockSentry(BlockSentry&&) = delete;
    BlockSentry& operator=(BlockSentry&&) = delete;
};

}