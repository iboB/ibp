// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <doctest/doctest.h>
#include <ibp/StringBuf.hpp>

using ibp::StringBuf;

TEST_CASE("StringBuf") {
    StringBuf sbuf;
    CHECK(sbuf.buf().empty());

    auto asdf = sbuf.addString("asdf");
    CHECK(asdf.begin == 0);
    CHECK(asdf.length == 4);
    auto xyz = sbuf.addString("xxyyzz");
    CHECK(xyz.begin == 5);
    CHECK(xyz.length == 6);
    CHECK(sbuf.buf().size() == 12);

    CHECK(sbuf.getString(asdf) == "asdf");
    CHECK(sbuf.getString(xyz) == "xxyyzz");
}
