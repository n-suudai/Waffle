

#include "memory/heap.h"
#include <cassert>


namespace waffle {
namespace memory {


MemoryTracker& MemoryTracker::get()
{
    static MemoryTracker instance;
    return instance;
}

MemoryTracker::MemoryTracker()
    : m_protection()
    , m_allocations()
    , m_nextAllocationBookmark(1)
{}

MemoryTracker::~MemoryTracker()
{
    // ※この時点で残っているAllocHeaderはリークしている
}

AllocHeader* MemoryTracker::recordAllocation(
    void* pBlock,
    wfl::size_t bytes,
    const char* file,
    wfl::int32_t line,
    const char* function,
    Heap* pHeap)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    wfl::size_t backTraceHash = 0;
    //if (AllocHeader::isEnabled(HeaderInfoFlagBits::BackTraceHash))
    //{
    //    // TODO : スタックトレースの実装
    //    backTraceHash = StackTrace::CaptureStackTraceHash();
    //}

    AllocHeader* pAllocHeader = new AllocHeader();
    pAllocHeader->record(
        pBlock,
        bytes,
        file,
        line,
        function,
        backTraceHash,
        m_nextAllocationBookmark,
        pHeap
    );

    m_allocations[pBlock] = pAllocHeader;

    // リンクリストを構築
    pHeap->addAllocation(pAllocHeader);

    m_nextAllocationBookmark++;

    return pAllocHeader;
}

void MemoryTracker::recordDeallocation(void* pBlock, Heap* pHeap)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    auto it = m_allocations.find(pBlock);

    if (it != m_allocations.end())
    {
        // シグネチャをチェック
        assert(it->second->isValidSignature());

        // リンクリストから切り離す
        pHeap->eraseAllocation(it->second);

        delete it->second;

        m_allocations.erase(it);
    }
}

wfl::size_t MemoryTracker::getAllocationBookmark() const
{
    return m_nextAllocationBookmark;
}


} // namespace memory
} // namespace waffle

