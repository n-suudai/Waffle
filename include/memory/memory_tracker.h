#pragma once


#include "memory/stl/stl_allocator_without_tracking.h"
#include <map>
#include <mutex>


namespace waffle {
namespace memory {


class Heap;

class AllocHeader;

class MemoryTracker final
{
    WFL_NO_NEW_DELETE;
public:
    template<typename Key, typename T, typename Compare = wfl::less<Key>,
        typename Alloc = STLAllocator_WithoutTracking<wfl::pair<const Key, T>>>
    using MapWithoutTracking = wfl::map<Key, T, Compare, Alloc>;

    using TrackMap = MapWithoutTracking<void*, AllocHeader*>;

    [[nodiscard]] static MemoryTracker& get();

    AllocHeader* recordAllocation(
        void* pBlock,
        wfl::size_t bytes,
        const char* file,
        wfl::int32_t line,
        const char* function,
        Heap* pHeap);

    void recordDeallocation(void* pBlock, Heap* pHeap);

    [[nodiscard]] wfl::size_t getAllocationBookmark() const;

private:
    MemoryTracker();
    ~MemoryTracker();

private:
    wfl::recursive_mutex m_protection;
    TrackMap m_allocations;
    wfl::size_t m_nextAllocationBookmark;
};


} // namespace memory
} // namespace waffle

