#include "memory/heap_factory.h"
#include "memory/heap.h"
#include "memory/heap_walk.h"
#include <cassert>


namespace waffle {
namespace memory {


HeapFactory& HeapFactory::get()
{
    static HeapFactory instance;
    return instance;
}

HeapFactory::HeapFactory()
{
    initialize();
}

HeapFactory::~HeapFactory()
{}

void HeapFactory::initialize()
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    for (auto& heap : m_heaps)
    {
        heap.initialize();
    }

    m_pRootHeap = createNewHeap(s_pRootHeapName);
    m_pDefaultHeap = createHeap(s_pDefaultHeapName);
}

Heap* HeapFactory::getRootHeap()
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    if (m_pRootHeap == nullptr)
    {
        initialize();
    }

    return m_pRootHeap;
}

Heap* HeapFactory::getDefaultHeap()
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    if (m_pDefaultHeap == nullptr)
    {
        initialize();
    }

    return m_pDefaultHeap;
}

Heap* HeapFactory::createHeap(
    const char* name,
    const char* parentName)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    if (m_pRootHeap == nullptr)
    {
        initialize();
    }

    // 親ヒープを探す
    Heap* pParent = findHeap(parentName);
    if (pParent == nullptr)
    { // 見つからなければ Root 以下に作成
        pParent = createNewHeap(parentName);
        pParent->attachTo(m_pRootHeap);
    }

    // ヒープを探す
    Heap* pHeap = findHeap(name);
    if (pHeap == nullptr)
    { // 見つからなければ作成
        pHeap = createNewHeap(name);
    }

    // pParent以下にアタッチ
    pHeap->attachTo(pParent);

    return pHeap;
}

Heap* HeapFactory::createHeap(const char* name)
{
    return createHeap(name, s_pRootHeapName);
}

Heap* HeapFactory::findHeap(const char* name)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    for (auto& heap : m_heaps)
    {
        if (heap.isActive() && !_stricmp(name, heap.getName()))
        {
            return &heap;
        }
    }

    return nullptr;
}

Heap* HeapFactory::createNewHeap(const char* name)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    for (auto& heap : m_heaps)
    {
        if (!heap.isActive())
        {
            heap.activate(name);
            return &heap;
        }
    }

    assert(false &&
        "ヒープの作成に失敗しました。作成可能なヒープの数を超えています。");
    return nullptr;
}

// リークのチェック関数
void HeapFactory::reportMemoryLeaks(
    IMemoryLeakReporter* pReporter,
    wfl::size_t bookmarkStart,
    wfl::size_t bookmarkEnd) const
{
    assert(pReporter != nullptr);

    pReporter->beginReport();

    for (auto& heap : m_heaps)
    {
        if (heap.isActive())
        {
            heap.reportMemoryLeaks(pReporter, bookmarkStart, bookmarkEnd);
        }
    }

    pReporter->endReport();
}

void HeapFactory::reportMemoryLeaks(
    IMemoryLeakReporter* pReporter,
    wfl::size_t bookmarkStart)
{
    reportMemoryLeaks(
        pReporter,
        bookmarkStart,
        MemoryTracker::get().getAllocationBookmark());
}

// メモリ情報
void HeapFactory::reportHeapTreeStats(
    IHeapTreeStatsReporter* pReporter)
{
    assert(pReporter != nullptr);

    pReporter->beginReport();

    m_pRootHeap->reportTreeStats(pReporter, 0);

    pReporter->endReport();
}

// メモリ破壊のチェック関数
void HeapFactory::reportMemoryAssertions(
    IMemoryAssertionReporter* pReporter,
    wfl::size_t bookmarkStart,
    wfl::size_t bookmarkEnd) const
{
    assert(pReporter != nullptr);

    pReporter->beginReport();

    for (auto& heap : m_heaps)
    {
        if (heap.isActive())
        {
            heap.reportMemoryAssertions(pReporter, bookmarkStart, bookmarkEnd);
        }
    }

    pReporter->endReport();
}

void HeapFactory::reportMemoryAssertions(
    IMemoryAssertionReporter* pReporter,
    wfl::size_t bookmarkStart) const
{
    reportMemoryAssertions(
        pReporter,
        bookmarkStart,
        MemoryTracker::get().getAllocationBookmark());
}


} // namespace memory
} // namespace waffle

