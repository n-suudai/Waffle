#pragma once


#include "memory/policy.h"
#include "memory/global_heap.h"
#include "memory/stl.h"
#include "memory/heap_walk.h"
#include "memory/heap_factory.h"

namespace waffle {
namespace memory {


bool initialize();

void finalize();


} // namespace memory
} // namespace waffle

