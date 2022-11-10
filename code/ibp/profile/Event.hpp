#pragma once
#include "../API.h"

namespace ibp::profile
{
class EntryDesc;
// implemented in ProfileImpl.cpp
IBP_API void newEvent(const EntryDesc& desc);
}
