// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <cstdio>
#include <ibp/Profile.hpp>

int main() {
    IBPROFILE_FUNC();
    printf("%s\n", __FUNCSIG__);
    return 0;
}
