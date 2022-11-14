// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <splat/symbol_export.h>
#include <iosfwd>

#if IBP_CVT_TRACE_SHARED
#   if BUILDING_IBP_CVT_TRACE
#       define IBP_CVT_TRACE_API SYMBOL_EXPORT
#   else
#       define IBP_CVT_TRACE_API SYMBOL_IMPORT
#   endif
#else
#   define IBP_CVT_TRACE_API
#endif

// convert to trace json
// https://docs.google.com/document/d/1CvAClvFfyA5R-PhYUmn5OOQtYMH4h6I0nSsKchNAySU/preview

// viewer
// https://ui.perfetto.dev/

namespace ibp {
class ProfileDump;
IBP_CVT_TRACE_API void ProfileDump_toTraceJson(const ProfileDump& dump, std::ostream& out);
}
