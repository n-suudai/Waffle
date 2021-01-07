#pragma once


#include "system/common/types.h"


namespace waffle {
namespace memory {


class Heap;

class AllocHeader;

// メモリリーク
class IMemoryLeakReporter
{
public:
    virtual ~IMemoryLeakReporter() = default;

    virtual void beginReport() = 0;

    virtual void report(const Heap* pHeap, const AllocHeader* pAllocHeader) = 0;

    virtual void endReport() = 0;
};

// メモリ情報
class IHeapTreeStatsReporter
{
public:
    struct HeapTreeStats
    {
        wfl::size_t totalBytes;
        wfl::size_t peakBytes;
        wfl::size_t instanceCount;

        inline HeapTreeStats() : totalBytes(0), peakBytes(0), instanceCount(0)
        {}
    };

    virtual ~IHeapTreeStatsReporter() = default;

    virtual void beginReport() = 0;

    virtual void report(
        wfl::int32_t depth,
        const Heap* pHeap,
        const HeapTreeStats& local,
        const HeapTreeStats& total) = 0;

    virtual void endReport() = 0;
};

// メモリ破壊
class IMemoryAssertionReporter
{
public:
    virtual ~IMemoryAssertionReporter() = default;

    virtual void beginReport() = 0;

    virtual void report(const Heap* pHeap, const AllocHeader* pAllocHeader) = 0;

    virtual void endReport() = 0;
};


} // namespace memory
} // namespace waffle