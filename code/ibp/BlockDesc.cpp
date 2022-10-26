#include "BlockDesc.hpp"

#define XXH_STATIC_LINKING_ONLY
#define XXH_IMPLEMENTATION
#include <xxhash.h>

namespace ibp
{

BlockDesc::BlockDesc(const char* l)
    : label(l)
{
    XXH64_state_t hstate;
    XXH64_reset(&hstate, 0);
    XXH64_update(&hstate, label.data(), label.length());
    hash = XXH64_digest(&hstate);
}

}