#include <cassert>
#include "memory.h"
#include "memory/alloc_header.h"

#include "memory/heap_walk.h"
#include "memory/heap_factory.h"


#include "common/platform/win32.h"


#define DebugPrintf(format, ...)              \
    {                                         \
        char buff[256];                       \
        sprintf_s(buff, format, __VA_ARGS__); \
        ::OutputDebugStringA(buff);             \
    }


namespace waffle {
namespace memory {


class TreeStatsReporter : public IHeapTreeStatsReporter
{
public:
    void beginReport() override
    {
        DebugPrintf("MEMORY INFORMATION\n");
        DebugPrintf(
            "                            Local                 Total\n");
        DebugPrintf(
            "Name                  Memory  Peak  Inst   Memory  Peak  Inst\n");
    }

    void report(
        wfl::int32_t depth,
        const Heap* pHeap,
        const HeapTreeStats& local,
        const HeapTreeStats& total) override
    {
        for (int i = 0; i < depth; ++i)
        {
            DebugPrintf("..");
        }

        int spacing = 20 - depth * 2;
        DebugPrintf("%-*s %6zu %6zu %5zu  %6zu %6zu %5zu\n", spacing,
            pHeap->getName(), local.totalBytes, local.peakBytes,
            local.instanceCount, total.totalBytes, total.peakBytes,
            total.instanceCount);
    }

    void endReport() override
    {}
};

class LeakReporter : public IMemoryLeakReporter
{
public:
    LeakReporter() : m_leakCount(0)
    {}

    void beginReport() override
    {
        m_leakCount = 0;
    }

    void report(const Heap* pHeap, const AllocHeader* pAllocHeader) override
    {
        assert(pHeap != nullptr);
        assert(pAllocHeader != nullptr);

        //DebugPrintf(
        //    "%s(%d)\n"
        //    "{ heap=\"%s\" address=0x%p size=%zubyte time=%s "
        //    "backTraceHash=0x%016llX bookmark=%llX }\n"
        //    "[ %08X ]\n",
        //    pAllocHeader->getFileName(), pAllocHeader->getLine(), pHeap->getName(),
        //    pAllocHeader->getBlock(), pAllocHeader->getBytes(),
        //    /*pAllocation->time*/ "", pAllocHeader->getBackTraceHash(),
        //    pAllocHeader->getBookmark(), pAllocHeader->getSignature());

        m_leakCount++;
    }

    void endReport() override
    {
        if (m_leakCount > 0)
        {
            DebugPrintf("%zu memory leaks found!\n", m_leakCount);
        }
        else
        {
            DebugPrintf("No memory leaks detected.\n");
        }
    }

public:
    size_t m_leakCount;
};

class AssertionReporter : public IMemoryAssertionReporter
{
public:
    AssertionReporter() : m_assertionCount(0)
    {}

    void beginReport() override
    {
        m_assertionCount = 0;
    }

    void report(const Heap* pHeap, const AllocHeader* pAllocHeader) override
    {
        assert(pHeap != nullptr);
        assert(pAllocHeader != nullptr);

        DebugPrintf(
            "%s(%d)\n"
            "{ heap=\"%s\" address=0x%p size=%zubyte time=%s "
            "backTraceHash=0x%016llX bookmark=%llX }\n"
            "[ %08X ]\n",
            pAllocHeader->getFileName(), pAllocHeader->getLine(), pHeap->getName(),
            pAllocHeader->getBlock(), pAllocHeader->getBytes(),
            /*pAllocation->time*/ "", pAllocHeader->getBackTraceHash(),
            pAllocHeader->getBookmark(), pAllocHeader->getSignature());

        m_assertionCount++;
    }

    void endReport() override
    {
        if (m_assertionCount > 0)
        {
            DebugPrintf("%zu memory assertions occurred!\n", m_assertionCount);
        }
        else
        {
            DebugPrintf("No memory assertions detected.\n");
        }
    }

public:
    size_t m_assertionCount;
};

void PrintDebug_Report_HeapTreeStats()
{
    TreeStatsReporter statsReporter;
    HeapFactory::get().reportHeapTreeStats(&statsReporter);
}
void PrintDebug_Report_MemoryLeaks()
{
    LeakReporter leakReporter;
    HeapFactory::get().reportMemoryLeaks(&leakReporter);
}
void PrintDebug_Report_MemoryAssertions()
{
    AssertionReporter assertionReporter;
    HeapFactory::get().reportMemoryAssertions(&assertionReporter);
}
void PrintDebug_Report_MemoryAll()
{
    PrintDebug_Report_HeapTreeStats();
    PrintDebug_Report_MemoryLeaks();
    PrintDebug_Report_MemoryAssertions();
}

bool initialize()
{
    HeaderInfoFlags flags = HeaderInfoFlagBits::All;
    bool result =  AllocHeader::initialize(flags);

    void* p0 = WFL_GLOBAL_HEAP_MALLOC(128);
    void* p1 = WFL_GLOBAL_HEAP_MALLOC(128);
    void* p2 = WFL_GLOBAL_HEAP_MALLOC(128);
    void* p3 = WFL_GLOBAL_HEAP_MALLOC(128);


    PrintDebug_Report_MemoryAll();


    WFL_GLOBAL_HEAP_FREE(p0);
    WFL_GLOBAL_HEAP_FREE(p1);
    WFL_GLOBAL_HEAP_FREE(p2);
    WFL_GLOBAL_HEAP_FREE(p3);


    PrintDebug_Report_MemoryAll();

    return result;
}

void finalize()
{

}


} // namespace memory
} // namespace waffle

