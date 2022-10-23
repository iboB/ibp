// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <splat/symbol_export.h>

#if IBP_SHARED
#   if BUILDING_IBP
#       define IBP_API SYMBOL_EXPORT
#   else
#       define IBP_API SYMBOL_IMPORT
#   endif
#else
#   define IBP_API
#endif
