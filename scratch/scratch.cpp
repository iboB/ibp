// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <ibp/Profile.hpp>
#include <ibp/Frame.hpp>
#include <ibp/FrameSentry.hpp>

#include <iostream>
#include <thread>

void bar() {
    IBPROFILE_FUNC();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    IBPROFILE_BLOCK_BEGIN("block");
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    IBPROFILE_BLOCK_END();
}

void foo(int i) {
    IBPROFILE_FUNC();
    std::this_thread::sleep_for(std::chrono::milliseconds(i * 10));
    if (i == 2) {
        bar();
    }
}

int main() {
    ibp::Frame frame("main");
    {
        ibp::FrameSentry f(frame);
        for (int i = 0; i < 3; ++i) {
            foo(i + 1);
        }
    }
    frame.dump(std::cout);
    return 0;
}
