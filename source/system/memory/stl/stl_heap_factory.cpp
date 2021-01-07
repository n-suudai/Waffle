#include "system/memory/stl/stl_heap_factory.h"
#include "system/memory/heap_factory.h"


namespace waffle {
namespace memory {


Heap* getHeapSTL()
{
    return HeapFactory::get().createHeap("STL");
}


} // namespace memory
} // namespace waffle

