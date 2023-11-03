// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <splat/symbol_export.h>
#include <iosfwd>

#if IBP_CVT_TREE_SHARED
#   if BUILDING_IBP_CVT_TREE
#       define IBP_CVT_TREE_API SYMBOL_EXPORT
#   else
#       define IBP_CVT_TREE_API SYMBOL_IMPORT
#   endif
#else
#   define IBP_CVT_TREE_API
#endif

namespace ibp {
class ProfileDump;
IBP_CVT_TREE_API void ProfileDump_toTree(const ProfileDump& dump);
}
