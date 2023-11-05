// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <ibp/Profile.hpp>
#include <ibp/profile/Frame.hpp>
#include <ibp/profile/FrameSentry.hpp>

#include <ibp/ProfileDump.hpp>
#include <ibp/DumpToTrace.hpp>
#include <ibp/PrintDump.hpp>

#include <iostream>
#include <fstream>
#include <thread>
#include <vector>

void bar() {
    IBPROFILE_FUNC();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    IBPROFILE_BLOCK_BEGIN("block");
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    IBPROFILE_BLOCK_END();
    IBPROFILE_EVENT("endbar");
}

void foo(int i) {
    IBPROFILE_FUNC(ibp::profile::Int(i));
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
        ibp::profile::FrameSentry _f(frame);
        for (int i = 0; i < 3; ++i) {
            ibp::profile::FrameSentry _f2(frame);
            foo(i + 1);
        }
    }
    auto d = frame.dump();
    ibp::ProfileDump_print(d, std::cout);

    std::ofstream fout("foo.json");
    ibp::ProfileDump_toTraceJson(d, fout);

    return 0;
}
