// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <ibp/Profile.hpp>
#include <ibp/profile/Frame.hpp>
#include <ibp/profile/FrameSentry.hpp>

#include <iostream>
#include <thread>

void bar() {
    IBPROFILE_FUNC();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    IBPROFILE_BLOCK_BEGIN("block");
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    IBPROFILE_BLOCK_END();
    IBPROFILE_EVENT("endbar");
}

void foo(int i) {
    IBPROFILE_FUNC();
    std::this_thread::sleep_for(std::chrono::milliseconds(i * 10));
    if (i == 2) {
        bar();
    }
    if (i == 1) {
        IBPROFILE_EVENT("foo 1");
    }
}

int main() {
    ibp::profile::Frame frame("main");
    frame.setEnabled();
    {
        ibp::profile::FrameSentry f(frame);
        for (int i = 0; i < 3; ++i) {
            ibp::profile::FrameSentry f(frame);
            foo(i + 1);
        }
    }
    frame.dump(std::cout);
    return 0;
}
