#pragma once
#include "API.h"

namespace ibp
{
class EventDesc;
namespace profile
{
// implemented in ProfileImpl.cpp
IBP_API void newEvent(const EventDesc& desc);
}
}
