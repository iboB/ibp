// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <splat/symbol_export.h>
#include <iosfwd>

#if IBP_PRINT_DUMP_SHARED
#   if BUILDING_IBP_PRINT_DUMP
#       define IBP_PRINT_DUMP_API SYMBOL_EXPORT
#   else
#       define IBP_PRINT_DUMP_API SYMBOL_IMPORT
#   endif
#else
#   define IBP_PRINT_DUMP_API
#endif

namespace ibp {
// human readable print of a profile dump
class ProfileDump;
IBP_PRINT_DUMP_API void ProfileDump_print(const ProfileDump& dump, std::ostream& out);
}
