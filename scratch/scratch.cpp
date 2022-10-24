// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <ibp/Profile.hpp>
#include <ibp/Frame.hpp>

#include <iostream>
#include <thread>

void bar() {
    IBPROFILE_FUNC();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

void foo(int i) {
    IBPROFILE_FUNC();
    std::this_thread::sleep_for(std::chrono::milliseconds(i * 10));
    if (i == 2) {
        bar();
    }
}

int main() {
    ibp::Frame frame;
    ibp::Instance::beginFrame(frame);
    for (int i = 0; i < 3; ++i) {
        foo(i + 1);
    }
    frame.dump(std::cout);
    return 0;
}
