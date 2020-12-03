
#include "memory/global_heap.h"
#include "memory/heap_factory.h"


namespace waffle {
namespace memory {


// 追跡あり
Heap* GlobalHeapWithTracking::s_pHeap = nullptr;


Heap* GlobalHeapWithTracking::getHeap()
{
    if (!s_pHeap)
    {
        s_pHeap = HeapFactory::get().createHeap("Global");
    }
    return s_pHeap;
}


} // namespace memory
} // namespace waffle

