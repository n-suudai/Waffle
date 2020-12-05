#pragma once


#include "common/utility/no_new_delete.h"
#include "memory/alloc_header.h"
#include "memory/memory_tracker.h"
#include <functional>


namespace waffle {
namespace memory {

class AllocHeader;
class IMemoryLeakReporter;
class IHeapTreeStatsReporter;
class IMemoryAssertionReporter;


class Heap final
{
    WFL_NO_NEW_DELETE;
public:
    Heap();

    ~Heap();

    void initialize();

    void activate(const char* name);

    void deactivate();

    const char* getName() const;

    bool isActive() const;

    template <typename Policy>
    inline void* allocate(
        wfl::size_t bytes,
        const char* file,
        wfl::int32_t line,
        const char* function);

    template <typename Policy>
    inline void* allocateAligned(
        wfl::size_t bytes,
        wfl::size_t alignment,
        const char* file,
        wfl::int32_t line,
        const char* function);

    template <typename Policy>
    inline void deallocate(void* pBlock);

    template <typename Policy>
    inline void deallocateAligned(void* pBlock, wfl::size_t alignment);

    // リンクリストを構築
    void addAllocation(AllocHeader* pAllocation);

    // リンクリストから切り離す
    void eraseAllocation(AllocHeader* pAllocation);

    // 親子関係の構築をする関数
    void attachTo(Heap* pParent);

    // リークのチェック関数
    void reportMemoryLeaks(
        IMemoryLeakReporter* pReporter,
        wfl::size_t bookmarkStart,
        wfl::size_t bookmarkEnd) const;

    // 情報収集のための関数
    void reportTreeStats(
        IHeapTreeStatsReporter* pAccumulator,
        wfl::int32_t depth) const;

    // メモリ破壊のチェック関数
    void reportMemoryAssertions(
        IMemoryAssertionReporter* pReporter,
        wfl::size_t bookmarkStart,
        wfl::size_t bookmarkEnd) const;

protected:
    void getTreeStats(
        wfl::size_t& totalBytes,
        wfl::size_t& totalPeakBytes,
        wfl::size_t& totalInstanceCount) const;

    void eachChild(const wfl::function<void(const Heap*)>& function) const;

private:
    wfl::recursive_mutex m_protection;

    enum
    {
        NAMELENGTH = 128
    };
    char m_name[NAMELENGTH];

    wfl::size_t m_totalAllocatedBytes;
    wfl::size_t m_peakAllocatedBytes;
    wfl::size_t m_allocatedInstanceCount;
    AllocHeader* m_pAllocHeader; // リンクリスト

    Heap* m_pParent;
    Heap* m_pFirstChild;
    Heap* m_pNextSibling;
    Heap* m_pPrevSibling;

    bool m_isActive;
};


template <typename Policy>
inline void* Heap::allocate(
    wfl::size_t bytes,
    const char* file,
    wfl::int32_t line,
    const char* function)
{
    wfl::lock_guard<wfl::recursive_mutex> lock(m_protection);

    // シグネチャサイズをプラス
    constexpr size_t signatureSize = sizeof(AllocHeader::Signature);

    // ポリシーを利用してメモリを確保
    void* pBlock = Policy::allocate(bytes + signatureSize);

    // トラッカーへ情報を登録
    MemoryTracker::get().recordAllocation(
        pBlock,
        bytes,
        file,
        line,
        function,
        this);

    return pBlock;
}

template <typename Policy>
inline void* Heap::allocateAligned(
    wfl::size_t bytes,
    wfl::size_t alignment,
    const char* file,
    wfl::int32_t line,
    const char* function)
{
    wfl::lock_guard<wfl::recursive_mutex> lock(m_protection);

    // シグネチャサイズをプラス
    constexpr size_t signatureSize = sizeof(AllocHeader::Signature);

    // ポリシーを利用してメモリを確保
    void* pBlock = Policy::allocateAligned(bytes + signatureSize, alignment);

    // トラッカーへ情報を登録
    MemoryTracker::get().recordAllocation(
        pBlock,
        bytes,
        file,
        line,
        function,
        this);

    return pBlock;
}

template <typename Policy>
inline void Heap::deallocate(void* pBlock)
{
    wfl::lock_guard<wfl::recursive_mutex> lock(m_protection);

    // トラッカーから情報を削除
    MemoryTracker::get().recordDeallocation(pBlock, this);

    // ポリシーを利用してメモリを破棄
    Policy::deallocate(pBlock);
}

template <typename Policy>
inline void Heap::deallocateAligned(void* pBlock, wfl::size_t alignment)
{
    wfl::lock_guard<wfl::recursive_mutex> lock(m_protection);

    // トラッカーから情報を削除
    MemoryTracker::get().recordDeallocation(pBlock, this);

    // ポリシーを利用してメモリを破棄
    Policy::deallocateAligned(pBlock, alignment);
}


} // namespace memory
} // namespace waffle

