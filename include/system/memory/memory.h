#pragma once


// Public Interfaces
#include "system/memory/policy/policy.h"
#include "system/memory/global_heap.h"
#include "system/memory/stl/stl.h"
#include "system/memory/heap_walk.h"
#include "system/memory/heap_factory.h"


namespace waffle {
namespace memory {


[[nodiscard]] bool initialize();

void finalize();


} // namespace memory
} // namespace waffle

