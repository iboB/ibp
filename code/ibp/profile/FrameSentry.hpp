// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "../API.h"

namespace ibp
{
class Frame;

namespace profile
{
// implemented in ProfileImpl.cpp
class IBP_API FrameSentry
{
    bool m_frameStarted;
public:
    // sets frame as current if frame is enabled
    FrameSentry(Frame& desc);
    ~FrameSentry();

    FrameSentry(const FrameSentry&) = delete;
    FrameSentry& operator=(const FrameSentry&) = delete;
    FrameSentry(FrameSentry&& other) noexcept
        : m_frameStarted(other.m_frameStarted)
    {
        other.m_frameStarted = false;
    }
    FrameSentry& operator=(FrameSentry&&) noexcept = delete;
};

}
}