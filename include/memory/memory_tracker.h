#pragma once


#include "common/types.h"
#include <map>
#include <mutex>


namespace waffle {
namespace memory {


class Heap;

class AllocHeader;

class MemoryTracker final
{
public:
    //typedef typename std::map<void*, Allocation*, std::less<void*>,
    //    STL::SHeapAllocator_NonTracking<
    //    std::pair<const void*, Allocation*>>> TrackMap;
    typedef wfl::map<void*, AllocHeader*> TrackMap;

    static MemoryTracker& get();

    AllocHeader* recordAllocation(
        void* pBlock,
        wfl::size_t bytes,
        const char* file,
        wfl::int32_t line,
        const char* function,
        Heap* pHeap);

    void recordDeallocation(void* pBlock, Heap* pHeap);

    wfl::size_t getAllocationBookmark() const;

private:
    MemoryTracker();
    ~MemoryTracker();

private:
    std::recursive_mutex m_protection;
    TrackMap m_allocations;
    wfl::size_t m_nextAllocationBookmark;
};


} // namespace memory
} // namespace waffle

