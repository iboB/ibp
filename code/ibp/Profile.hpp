// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "BlockDesc.hpp"
#include "Sentry.hpp"

#define I_IBPROFILE_BLOCK_DESC(label) static ::ibp::BlockDesc __ibpBlockDesc(label)

#define IBPROFILE_SCOPE(label) \
    I_IBPROFILE_BLOCK_DESC(label); \
    ::ibp::Sentry __ibpSentry(__ibpBlockDesc)

#define IBPROFILE_FUNC() \
    IBPROFILE_SCOPE(__func__)

