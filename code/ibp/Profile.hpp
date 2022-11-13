// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

#if !defined(IBPROFILE_DISABLED)

#include "profile/EntryDesc.hpp"
#include "profile/ProfileFuncs.hpp"

#define I_IBP_PP_CAT(a, b) I_IBP_PP_INTERNAL_CAT(a, b)
#define I_IBP_PP_INTERNAL_CAT(a, b) a##b

#define I_IBPROFILE_U_NAME(name) I_IBP_PP_CAT(name, __LINE__)

// block descriptions don't necessarily need to be thread_local
// in fact much of the report code would be simpler if they're not (for example dedup by address)
// but if we have them as simply static, they will incur a bigger cost on every block because of
// the need of thread sync, so we have them as thread local and dedup them by hash
#define I_IBPROFILE_EVENT_DESC(type, label) static thread_local ::ibp::profile::EntryDesc I_IBPROFILE_U_NAME(_ibp_blockDesc)(type, label)

#define I_PROFILE_BLOCK(type, label) \
    I_IBPROFILE_EVENT_DESC(type, label); \
    ::ibp::profile::BlockSentry I_IBPROFILE_U_NAME(_ibp_profileSentry)(I_IBPROFILE_U_NAME(_ibp_blockDesc))


#define IBPROFILE_FUNC() I_PROFILE_BLOCK(::ibp::profile::EntryDesc::Type::Function, __func__)

#define IBPROFILE_SCOPE(label) I_PROFILE_BLOCK(::ibp::profile::EntryDesc::Type::Block, label)

#define IBPROFILE_BLOCK_BEGIN(label) \
    I_IBPROFILE_EVENT_DESC(::ibp::profile::EntryDesc::Type::Block, label); \
    ::ibp::profile::newEvent(I_IBPROFILE_U_NAME(_ibp_blockDesc))

#define IBPROFILE_BLOCK_END() ::ibp::profile::endEvent()

#define IBPROFILE_EVENT(label) \
    I_IBPROFILE_EVENT_DESC(::ibp::profile::EntryDesc::Type::Event, label); \
    ::ibp::profile::newEvent(I_IBPROFILE_U_NAME(_ibp_blockDesc))

#else // IBPROFILE_DISABLED

#define IBPROFILE_FUNC(...)
#define IBPROFILE_SCOPE(...)
#define IBPROFILE_BLOCK_BEGIN(...)
#define IBPROFILE_BLOCK_END(...)
#define IBPROFILE_EVENT(...)

#endif