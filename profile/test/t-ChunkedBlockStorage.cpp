// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <doctest/doctest.h>
#include <ibp/profile/ChunkedBlockStorage.hpp>

#include <vector>

using ibp::profile::ChunkedBlockStorage;

TEST_CASE("test") {
    ChunkedBlockStorage<int> cbs(2);
    CHECK_FALSE(cbs.valid()); // not valid by default
    CHECK(cbs.empty()); // empty on ivalid is safe

    // iterate invalid is safe (and empty)
    using ivec = std::vector<int>;
    ivec val;
    for (auto& i : cbs) {
        val.push_back(i);
    }
    CHECK(val.empty());

    cbs.init();
    CHECK(cbs.valid()); // valid after initialize
    CHECK(cbs.empty()); // still empty

    for (auto& i : cbs) {
        val.push_back(i);
    }
    CHECK(val.empty()); // still empty after init

    cbs.emplace_back() = 1;
    cbs.emplace_back() = 2;
    CHECK_FALSE(cbs.empty()); // not empty

    std::vector<const int*> addr;

    for (auto& i : cbs) {
        addr.push_back(&i);
        val.push_back(i);
    }

    CHECK(val == ivec{1, 2}); // contents
    CHECK(addr[0] + 1 == addr[1]); // consecutive addresses

    cbs.emplace_back() = 3;
    cbs.emplace_back() = 4;
    cbs.emplace_back() = 5;

    std::vector<const int*> addr2;
    val.clear();
    for (auto& i : cbs) {
        addr2.push_back(&i);
        val.push_back(i);
    }

    CHECK(val == ivec{1, 2, 3, 4, 5}); // contents
    CHECK(addr[0] == addr2[0]); // first two addresses unchanged
    CHECK(addr[1] == addr2[1]);
    CHECK(addr2[1] + 1 != addr2[2]); // second pair is not after first
    CHECK(addr2[2] + 1 == addr2[3]); // second pair is consecutive
    CHECK(addr2[3] + 1 != addr2[4]); // third pair is not after second

    cbs.reset();
    CHECK(cbs.valid()); // valid after reset
    CHECK(cbs.empty());

    val.clear();
    for (auto& i : cbs) {
        val.push_back(i);
    }
    CHECK(val.empty()); // empty after reset

    cbs.emplace_back() = 10;
    cbs.emplace_back() = 20;
    cbs.emplace_back() = 30;
    cbs.emplace_back() = 40;
    cbs.emplace_back() = 50;

    addr.clear();
    val.clear();
    for (auto& i : cbs) {
        addr.push_back(&i);
        val.push_back(i);
    }

    CHECK(val == ivec{10, 20, 30, 40, 50}); // contents
    CHECK(addr == addr2); // addresses preserved

    auto cbs2 = std::move(cbs);
    CHECK_FALSE(cbs.valid()); // not valid after move
    CHECK(cbs.empty());
    CHECK(cbs2.valid());

    addr.clear();
    val.clear();
    for (auto& i : cbs2) {
        addr.push_back(&i);
        val.push_back(i);
    }

    CHECK(val == ivec{10, 20, 30, 40, 50}); // contents
    CHECK(addr == addr2); // addresses preserved

    cbs2.reset();

    auto cbs3 = std::move(cbs2);
    CHECK(cbs3.valid());
    CHECK(cbs3.empty());

    cbs3.emplace_back() = 6;
    cbs3.emplace_back() = 7;
    cbs3.emplace_back() = 8;
    cbs3.emplace_back() = 9;
    cbs3.emplace_back() = 10;

    addr.clear();
    val.clear();
    for (auto& i : cbs3) {
        addr.push_back(&i);
        val.push_back(i);
    }

    CHECK(val == ivec{6, 7, 8, 9, 10}); // contents
    CHECK(addr == addr2); // addresses preserved
}
