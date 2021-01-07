#pragma once

#include "system/common/utility/no_new_delete.h"
#include "system/memory/heap.h"
#include <array>


namespace waffle {
namespace memory {


class HeapFactory final
{
    WFL_NO_NEW_DELETE;
public:
    [[nodiscard]] static HeapFactory& get();

    void initialize();

    [[nodiscard]] Heap* getRootHeap();

    [[nodiscard]] Heap* getDefaultHeap();

    [[nodiscard]] Heap* createHeap(
        const char* name,
        const char* parentName);

    [[nodiscard]] Heap* createHeap(const char* name);

    [[nodiscard]] Heap* findHeap(const char* name);

    [[nodiscard]] Heap* createNewHeap(const char* name);

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
    HeapFactory();

    ~HeapFactory();

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

