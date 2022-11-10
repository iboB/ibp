// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <doctest/doctest.h>
#include <ibp/profile/ChunkedBlockStorage.hpp>

#include <vector>

using ibp::profile::ChunkedBlockStorage;

TEST_CASE("test")
{
    ChunkedBlockStorage<int> c(2);
    CHECK_FALSE(c.valid()); // not valid by default
    CHECK(c.empty()); // empty on ivalid is safe

    // iterate invalid is safe (and empty)
    using ivec = std::vector<int>;
    ivec val;
    for (auto& i : c)
    {
        val.push_back(i);
    }
    CHECK(val.empty());

    c.init();
    CHECK(c.valid()); // valid after initialize
    CHECK(c.empty()); // still empty

    for (auto& i : c)
    {
        val.push_back(i);
    }
    CHECK(val.empty()); // still empty after init

    c.emplace_back() = 1;
    c.emplace_back() = 2;
    CHECK_FALSE(c.empty()); // not empty

    std::vector<const int*> addr;

    for (auto& i : c)
    {
        addr.push_back(&i);
        val.push_back(i);
    }

    CHECK(val == ivec{1, 2}); // contents
    CHECK(addr[0] + 1 == addr[1]); // consecutive addresses

    c.emplace_back() = 3;
    c.emplace_back() = 4;
    c.emplace_back() = 5;

    std::vector<const int*> addr2;
    val.clear();
    for (auto& i : c)
    {
        addr2.push_back(&i);
        val.push_back(i);
    }

    CHECK(val == ivec{1, 2, 3, 4, 5}); // contents
    CHECK(addr[0] == addr2[0]); // first two addresses unchanged
    CHECK(addr[1] == addr2[1]);
    CHECK(addr2[1] + 1 != addr2[2]); // second pair is not after first
    CHECK(addr2[2] + 1 == addr2[3]); // second pair is consecutive
    CHECK(addr2[3] + 1 != addr2[4]); // third pair is not after second

    c.reset();
    CHECK(c.valid()); // valid after reset
    CHECK(c.empty());

    val.clear();
    for (auto& i : c)
    {
        val.push_back(i);
    }
    CHECK(val.empty()); // empty after reset

    c.emplace_back() = 10;
    c.emplace_back() = 20;
    c.emplace_back() = 30;
    c.emplace_back() = 40;
    c.emplace_back() = 50;

    addr.clear();
    val.clear();
    for (auto& i : c)
    {
        addr.push_back(&i);
        val.push_back(i);
    }

    CHECK(val == ivec{10, 20, 30, 40, 50}); // contents
    CHECK(addr == addr2); // addresses preserved

    auto c2 = std::move(c);
    CHECK_FALSE(c.valid()); // not valid after move
    CHECK(c.empty());
    CHECK(c2.valid());

    addr.clear();
    val.clear();
    for (auto& i : c2)
    {
        addr.push_back(&i);
        val.push_back(i);
    }

    CHECK(val == ivec{10, 20, 30, 40, 50}); // contents
    CHECK(addr == addr2); // addresses preserved

    c2.reset();

    auto c3 = std::move(c2);
    CHECK(c3.valid());
    CHECK(c3.empty());

    c3.emplace_back() = 6;
    c3.emplace_back() = 7;
    c3.emplace_back() = 8;
    c3.emplace_back() = 9;
    c3.emplace_back() = 10;

    addr.clear();
    val.clear();
    for (auto& i : c3)
    {
        addr.push_back(&i);
        val.push_back(i);
    }

    CHECK(val == ivec{6, 7, 8, 9, 10}); // contents
    CHECK(addr == addr2); // addresses preserved
}
