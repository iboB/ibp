// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <doctest/doctest.h>
#include <ibp/impl/ChunkedBlockStorage.hpp>

#include <vector>

using ibp::impl::ChunkedBlockStorage;

TEST_CASE("basic")
{
    ChunkedBlockStorage<int> c(2);
    CHECK_FALSE(c.valid());
    c.init();
    CHECK(c.valid());

    c.emplace_back() = 1;
    c.emplace_back() = 2;

    std::vector<const int*> addr;
    using ivec = std::vector<int>;
    ivec val;

    for (auto& i : c)
    {
        addr.push_back(&i);
        val.push_back(i);
    }

    CHECK(val == ivec{1, 2});
    CHECK(addr[0] + 1 == addr[1]);

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

    CHECK(val == ivec{1, 2, 3, 4, 5});
    CHECK(addr[0] == addr2[0]);
    CHECK(addr[1] == addr2[1]);
    CHECK(addr2[1] + 1 != addr2[2]);
    CHECK(addr2[2] + 1 == addr2[3]);
    CHECK(addr2[3] + 1 != addr2[4]);
}
