#include "memory/heap.h"
#include "memory/heap_walk.h"
#include <cassert>


namespace waffle {
namespace memory {


Heap::Heap()
    : m_protection()
    , m_totalAllocatedBytes(0)
    , m_peakAllocatedBytes(0)
    , m_allocatedInstanceCount(0)
    , m_pAllocHeader(nullptr)
    , m_pParent(nullptr)
    , m_pFirstChild(nullptr)
    , m_pNextSibling(nullptr)
    , m_pPrevSibling(nullptr)
    , m_isActive(false)
    , m_name()
{

}

Heap::~Heap()
{}

void Heap::initialize()
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    m_isActive = false;
    m_totalAllocatedBytes = 0;
    m_peakAllocatedBytes = 0;
    m_allocatedInstanceCount = 0;
    m_pAllocHeader = nullptr;
    m_pParent = nullptr;
    m_pFirstChild = nullptr;
    m_pNextSibling = nullptr;
    m_pPrevSibling = nullptr;
}

void Heap::activate(const char* name)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    assert(name != nullptr);
    assert(strlen(name) < NAMELENGTH);
    strcpy_s(m_name, name);
    m_isActive = true;
    m_totalAllocatedBytes = 0;
    m_peakAllocatedBytes = 0;
    m_allocatedInstanceCount = 0;
}

void Heap::deactivate()
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    strcpy_s(m_name, "");
    m_isActive = false;
    m_totalAllocatedBytes = 0;
    m_peakAllocatedBytes = 0;
    m_allocatedInstanceCount = 0;
}

const char* Heap::getName() const
{
    return m_name;
}

bool Heap::isActive() const
{
    return m_isActive;
}

// リンクリストを構築
void Heap::addAllocation(AllocHeader* pAllocHeader)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    pAllocHeader->addLink(m_pAllocHeader);

    m_pAllocHeader = pAllocHeader;

    // 確保サイズ情報更新
    m_totalAllocatedBytes += m_pAllocHeader->getBytes();

    if (m_totalAllocatedBytes > m_peakAllocatedBytes)
    {
        m_peakAllocatedBytes = m_totalAllocatedBytes;
    }

    m_allocatedInstanceCount++;
}

// リンクリストから切り離す
void Heap::eraseAllocation(AllocHeader* pAllocHeader)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    AllocHeader* pNextTop = pAllocHeader->deleteLink();
    if (pNextTop != nullptr)
    {
        assert(pAllocHeader == m_pAllocHeader);
        m_pAllocHeader = pNextTop;
    }

    // 確保サイズ情報更新
    m_totalAllocatedBytes -= pAllocHeader->getBytes();
    m_allocatedInstanceCount--;
}

// 親子関係の構築をする関数
void Heap::attachTo(Heap* pParent)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    assert(pParent != nullptr);

    if (pParent == m_pParent)
    {
        return;
    }

    // 1. 現在の親子関係を切り離す
    if (m_pPrevSibling != nullptr)
    {
        m_pPrevSibling->m_pNextSibling = m_pNextSibling;
    }

    if (m_pNextSibling != nullptr)
    {
        m_pNextSibling->m_pPrevSibling = m_pPrevSibling;
    }

    if (m_pParent != nullptr)
    {
        if (m_pParent->m_pFirstChild == this)
        {
            m_pParent->m_pFirstChild = m_pNextSibling;
        }
    }

    // 2. 新しく親子関係を構築
    m_pNextSibling = pParent->m_pFirstChild;
    m_pPrevSibling = nullptr;
    m_pParent = pParent;
    pParent->m_pFirstChild = this;
}

// リークのチェック関数
void Heap::memoryLeakCheck(
    IMemoryLeakReporter* pReporter,
    wfl::size_t bookmarkStart,
    wfl::size_t bookmarkEnd) const
{
    assert(pReporter != nullptr);

    AllocHeader* pAllocHeader = m_pAllocHeader;
    while (pAllocHeader != nullptr)
    {
        if (pAllocHeader->getBookmark() >= bookmarkStart &&
            pAllocHeader->getBookmark() <= bookmarkEnd)
        {
            pReporter->report(this, pAllocHeader);
        }
        pAllocHeader = pAllocHeader->getNext();
    }
}

// 情報収集のための関数
void Heap::reportTreeStats(
    IHeapTreeStatsReporter* pAccumulator,
    wfl::int32_t depth) const
{
    assert(pAccumulator != nullptr);

    IHeapTreeStatsReporter::HeapTreeStats local, total;

    local.totalBytes = m_totalAllocatedBytes;
    local.peakBytes = m_peakAllocatedBytes;
    local.instanceCount = m_allocatedInstanceCount;

    getTreeStats(total.totalBytes, total.peakBytes, total.instanceCount);

    pAccumulator->report(depth, this, local, total);

    eachChild(
        [pAccumulator, depth](const Heap* pChild)
        {
            pChild->reportTreeStats(pAccumulator, depth + 1);
        });
}

// メモリ破壊のチェック関数
void Heap::memoryAssertionCheck(
    IMemoryAssertionReporter* pReporter,
    wfl::size_t bookmarkStart,
    wfl::size_t bookmarkEnd) const
{
    assert(pReporter != nullptr);

    AllocHeader* pAllocHeader = m_pAllocHeader;
    while (pAllocHeader != nullptr)
    {
        if (pAllocHeader->getBookmark() >= bookmarkStart &&
            pAllocHeader->getBookmark() <= bookmarkEnd)
        {
            if (pAllocHeader->getSignature() != AllocHeader::SIGNATURE)
            {
                pReporter->report(this, pAllocHeader);
            }
        }
        pAllocHeader = pAllocHeader->getNext();
    }
}

void Heap::getTreeStats(
    wfl::size_t& totalBytes,
    wfl::size_t& totalPeakBytes,
    wfl::size_t& totalInstanceCount) const
{
    totalBytes += m_totalAllocatedBytes;
    totalPeakBytes += m_peakAllocatedBytes;
    totalInstanceCount += m_allocatedInstanceCount;

    Heap* pChild = m_pFirstChild;
    while (pChild != NULL)
    {
        pChild->getTreeStats(totalBytes, totalPeakBytes, totalInstanceCount);
        pChild = pChild->m_pNextSibling;
    }

    eachChild(
        [&totalBytes, &totalPeakBytes, &totalInstanceCount](const Heap* pChild)
        {
            pChild->getTreeStats(totalBytes, totalPeakBytes, totalInstanceCount);
        });
}

void Heap::eachChild(const wfl::function<void(const Heap*)>& function) const
{
    Heap* pChild = m_pFirstChild;
    while (pChild != nullptr)
    {
        function(pChild);
        pChild = pChild->m_pNextSibling;
    }
}


} // namespace memory
} // namespace waffle

