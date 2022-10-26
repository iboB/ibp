// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "BlockDesc.hpp"
#include "BlockSentry.hpp"

#define I_IBP_PP_CAT(a, b) I_IBP_PP_INTERNAL_CAT(a, b)
#define I_IBP_PP_INTERNAL_CAT(a, b) a##b

#define I_IBPROFILE_U_NAME(name) I_IBP_PP_CAT(name, __LINE__)

// block descriptions don't necessarily need to be thread_local
// in fact much of the report ode would be simpler if they're not (for example dedup by address)
// but if we have them as simply static they will incur a bigger cost on every block because of the need of thread sync
// so we have them as thread local and dedup them by hash
#define I_IBPROFILE_BLOCK_DESC(label) static thread_local ::ibp::BlockDesc I_IBPROFILE_U_NAME(_ibp_blockDesc)(label)

#define IBPROFILE_SCOPE(label) \
    I_IBPROFILE_BLOCK_DESC(label); \
    ::ibp::BlockSentry I_IBPROFILE_U_NAME(_ibp_profileSentry)(I_IBPROFILE_U_NAME(_ibp_blockDesc))

#define IBPROFILE_FUNC() IBPROFILE_SCOPE(__func__)

#define IBPROFILE_BLOCK_BEGIN(label) \
    I_IBPROFILE_BLOCK_DESC(label); \
    ::ibp::profile::beginBlock(I_IBPROFILE_U_NAME(_ibp_blockDesc))

#define IBPROFILE_BLOCK_END() ::ibp::profile::endTopBlock()
