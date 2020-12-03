#pragma once

#include "memory/heap.h"
#include "memory/policy.h"


namespace waffle {
namespace memory {


// 関数をインラインで定義しているのは、
// AllocatePolicy が memory/config.h の定義によって変化するため。
// cpp に移してしまうと、memory/config.h が変更されるたびにビルドし直す必要がある。


class Heap;

// 追跡あり
// Heap 経由でメモリ確保
class GlobalHeapWithTracking final
{
    WFL_NON_INSTANTIATABLE(GlobalHeapWithTracking);
private:
    static Heap* s_pHeap;
    static Heap* getHeap();

public:
    static inline void* heapMalloc(
        wfl::size_t bytes,
        const char* file,
        wfl::int32_t line,
        const char* function)
    {
        return getHeap()->allocate<AllocatePolicy>(
            bytes, file, line, function);
    }

    static inline void* heapMallocAligned(
        wfl::size_t bytes,
        wfl::size_t alignment,
        const char* file,
        wfl::int32_t line,
        const char* function)
    {
        return getHeap()->allocateAligned<AllocatePolicy>(
            bytes, alignment, file, line, function);
    }

    static inline void heapFree(
        void* pBlock)
    {
        getHeap()->deallocate<AllocatePolicy>(pBlock);
    }

    static void heapFreeAligned(
        void* pBlock,
        wfl::size_t alignment)
    {
        getHeap()->deallocateAligned<AllocatePolicy>(pBlock, alignment);
    }
};


// 追跡なし
// ポリシーから直接メモリ確保
class GlobalHeapWithoutTracking final
{
    WFL_NON_INSTANTIATABLE(GlobalHeapWithoutTracking);
public:
    static inline void* heapMalloc(
        wfl::size_t bytes)
    {
        return AllocatePolicy::allocate(bytes);
    }

    static inline void* heapMallocAligned(
        wfl::size_t bytes,
        wfl::size_t alignment)
    {
        return AllocatePolicy::allocateAligned(bytes, alignment);
    }

    static inline void heapFree(
        void* pBlock)
    {
        return AllocatePolicy::deallocate(pBlock);
    }

    static inline void heapFreeAligned(
        void* pBlock,
        wfl::size_t alignment)
    {
        return AllocatePolicy::deallocateAligned(pBlock, alignment);
    }
};


} // namespace memory
} // namespace waffle


#define WFL_GLOBAL_HEAP_MALLOC_WITH_TRACKING(bytes) \
waffle::memory::GlobalHeapWithTracking::heapMalloc((bytes), __FILE__, __LINE__, __FUNCTION__)

#define WFL_GLOBAL_HEAP_MALLOC_ALIGNED_WITH_TRACKING(bytes, alignment) \
waffle::memory::GlobalHeapWithTracking::heapMallocAligned((bytes), (alignment), __FILE__, __LINE__, __FUNCTION__)

#define WFL_GLOBAL_HEAP_FREE_WITH_TRACKING(pBlock) \
waffle::memory::GlobalHeapWithTracking::heapFree((pBlock))

#define WFL_GLOBAL_HEAP_FREE_ALIGNED_WITH_TRACKING(pBlock, alignment) \
waffle::memory::GlobalHeapWithTracking::heapFreeAligned((pBlock), (alignment))


#if WFL_USE_HEAP_TRACKING

#define WFL_GLOBAL_HEAP_MALLOC \
WFL_GLOBAL_HEAP_MALLOC_WITH_TRACKING

#define WFL_GLOBAL_HEAP_MALLOC_ALIGNED \
WFL_GLOBAL_HEAP_MALLOC_ALIGNED_WITH_TRACKING

#define WFL_GLOBAL_HEAP_FREE \
WFL_GLOBAL_HEAP_FREE_WITH_TRACKING

#define WFL_GLOBAL_HEAP_FREE_ALIGNED \
WFL_GLOBAL_HEAP_FREE_ALIGNED_WITH_TRACKING

#else

#define WFL_GLOBAL_HEAP_MALLOC(bytes) \
waffle::memory::GlobalHeapWithoutTracking::heapMalloc((bytes))

#define WFL_GLOBAL_HEAP_MALLOC_ALIGNED(bytes, alignment) \
waffle::memory::GlobalHeapWithoutTracking::heapMallocAligned((bytes), (alignment))

#define WFL_GLOBAL_HEAP_FREE(pBlock) \
waffle::memory::GlobalHeapWithoutTracking::heapFree((pBlock))

#define WFL_GLOBAL_HEAP_FREE_ALIGNED(pBlock, alignment) \
waffle::memory::GlobalHeapWithoutTracking::heapFreeAligned((pBlock), (alignment))

#endif

