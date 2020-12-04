#pragma once


#include "memory/config.h"


#if WFL_ALLOC_POLICY == WFL_ALLOC_POLICY_NED_POOLING

#include "memory/policy/policy_nedmalloc_pooling.h"

namespace waffle {
namespace memory {

typedef NedPoolingAllocatePolicy AllocatePolicy;

} // namespace memory
} // namespace waffle

#elif WFL_ALLOC_POLICY == WFL_ALLOC_POLICY_NED

#include "memory/policy/policy_nedmalloc.h"

namespace waffle {
namespace memory {

typedef NedAllocatePolicy AllocatePolicy;

} // namespace memory
} // namespace waffle

#elif WFL_ALLOC_POLICY == WFL_ALLOC_POLICY_STANDARD

#include "memory/policy/policy_standard.h"

namespace waffle {
namespace memory {

typedef StandardAllocatePolicy AllocatePolicy;

} // namespace memory
} // namespace waffle

#else

#error "does not defined alloc-policy."

#endif

