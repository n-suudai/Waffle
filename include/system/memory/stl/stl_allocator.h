#pragma once


#include "system/memory/config.h"

#if WFL_USE_HEAP_TRACKING

#include "system/memory/stl/stl_allocator_with_tracking.h"

namespace waffle {
namespace memory {

template<typename T>
using STLAllocator = STLAllocator_WithTracking<T>;

} // namespace memory
} // namespace waffle

#else

#include "system/memory/stl/stl_allocator_without_tracking.h"

namespace waffle {
namespace memory {

template<typename T>
using STLAllocator = STLAllocator_WithoutTracking<T>;

} // namespace memory
} // namespace waffle

#endif








