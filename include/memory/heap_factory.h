#pragma once


#include "memory/heap.h"
#include <array>


namespace waffle {
namespace memory {


class HeapFactory
{
public:
    static HeapFactory& get();

    HeapFactory();

    ~HeapFactory();

    void initialize();

    Heap* getRootHeap();

    Heap* getDefaultHeap();

    Heap* createHeap(
        const char* name,
        const char* parentName);

    Heap* createHeap(const char* name);

    Heap* findHeap(const char* name);

    Heap* createNewHeap(const char* name);

    // リークのチェック関数
    void reportMemoryLeaks(
        IMemoryLeakReporter* pReporter,
        wfl::size_t bookmarkStart,
        wfl::size_t bookmarkEnd) const;

    void reportMemoryLeaks(
        IMemoryLeakReporter* pReporter,
        wfl::size_t bookmarkStart = 0);

    // メモリ情報
    void reportHeapTreeStats(
        IHeapTreeStatsReporter* pReporter);

    // メモリ破壊のチェック関数
    void reportMemoryAssertions(
        IMemoryAssertionReporter* pReporter,
        wfl::size_t bookmarkStart,
        wfl::size_t bookmarkEnd) const;

    void reportMemoryAssertions(
        IMemoryAssertionReporter* pReporter,
        wfl::size_t bookmarkStart = 0) const;

private:
    static constexpr char s_pRootHeapName[] = "Root";
    static constexpr char s_pDefaultHeapName[] = "Default";
    static constexpr wfl::uint16_t MAXHEAPS = 256;

    wfl::recursive_mutex m_protection;
    wfl::array<Heap, MAXHEAPS> m_heaps;
    Heap* m_pRootHeap;
    Heap* m_pDefaultHeap;
};


} // namespace memory
} // namespace waffle

